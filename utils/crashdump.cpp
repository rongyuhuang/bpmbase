#include "crashdump.h"

#include <windows.h>

#include <dbghelp.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <eh.h>
#include <crtdbg.h>

// Minidump with stacks, PEB, TEB, and unloaded module list.
static const MINIDUMP_TYPE kSmallDumpType = static_cast<MINIDUMP_TYPE>(
    MiniDumpWithProcessThreadData | // Get PEB and TEB.
    MiniDumpWithHandleData | MiniDumpWithUnloadedModules); // Get unloaded modules when available.

// Minidump with all of the above, plus memory referenced from stack.
static const MINIDUMP_TYPE kLargerDumpType = static_cast<MINIDUMP_TYPE>(
    MiniDumpWithProcessThreadData | // Get PEB and TEB.
    MiniDumpWithHandleData | MiniDumpWithUnloadedModules | // Get unloaded modules when available.
    MiniDumpWithIndirectlyReferencedMemory); // Get memory referenced by stack.

// Large dump with all process memory.
static const MINIDUMP_TYPE kFullDumpType = static_cast<MINIDUMP_TYPE>(
    MiniDumpWithFullMemory | // Full memory from process.
    MiniDumpWithProcessThreadData | // Get PEB and TEB.
    MiniDumpWithHandleData | // Get all handle information.
    MiniDumpWithUnloadedModules); // Get unloaded modules when available.

#define PROCESS_CALLBACK_FILTER_ENABLED 0x1

BOOL
(WINAPI*
 MiniDumpWriteDumpPtr)(
     _In_ HANDLE hProcess,
     _In_ DWORD ProcessId,
     _In_ HANDLE hFile,
     _In_ MINIDUMP_TYPE DumpType,
     _In_opt_ PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
     _In_opt_ PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
     _In_opt_ PMINIDUMP_CALLBACK_INFORMATION CallbackParam
 );

BOOL
(WINAPI*
 SetProcessUserModeExceptionPolicyPtr)(
     _In_ DWORD PolicyFlags
 );

BOOL
(WINAPI*
 GetProcessUserModeExceptionPolicyPtr)(
     _Out_ LPDWORD PolicyFlags
 );

//////

static void CrashDumpFatal(const char* Format, ...);
static void CrashDumpCreate(EXCEPTION_POINTERS* ExceptionPointers);
static LONG CALLBACK CrashDumpExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo);
static void InvalidParameterHandler(const wchar_t* Expression, const wchar_t* Function, const wchar_t* File, unsigned int Line, uintptr_t Reserved);
static void TerminateHandler();
static void AbortHandler(int Signal);

/////

void bpm_CrashDumpInitialize(bool inexe)
{
    // If not running under a debugger, redirect purecall, terminate, abort, and
    // invalid parameter callbacks to force generate a dump.
    if(!IsDebuggerPresent())
    {
        _set_purecall_handler(TerminateHandler);                // https://msdn.microsoft.com/en-us/library/t296ys27.aspx
        _set_invalid_parameter_handler(InvalidParameterHandler);// https://msdn.microsoft.com/en-us/library/a9yf33zb.aspx
        set_terminate(TerminateHandler);                        // http://en.cppreference.com/w/cpp/error/set_terminate
        signal(SIGABRT, AbortHandler);                          // https://msdn.microsoft.com/en-us/library/xdkz3x12.aspx

        // disalbe crt assert report
        _CrtSetReportMode(_CRT_ASSERT, 0);
        _CrtSetReportMode(_CRT_ERROR, 0);
    }

    // If not exe, not SetUnhandledExceptionFilter
    if(!inexe){
        return;
    }

    // Get handles to kernel32 and dbghelp
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    HMODULE hDbghelp = LoadLibraryA("dbghelp.dll");

    if(hDbghelp)
        *(FARPROC*)&MiniDumpWriteDumpPtr = GetProcAddress(hDbghelp, "MiniDumpWriteDump");

    if(hKernel32)
    {
        *(FARPROC*)&SetProcessUserModeExceptionPolicyPtr = GetProcAddress(hKernel32, "SetProcessUserModeExceptionPolicy");
        *(FARPROC*)&GetProcessUserModeExceptionPolicyPtr = GetProcAddress(hKernel32, "GetProcessUserModeExceptionPolicy");
    }

    if(MiniDumpWriteDumpPtr)
        SetUnhandledExceptionFilter(CrashDumpExceptionHandler);

    // Ensure that exceptions are not swallowed when dispatching certain Windows
    // messages.
    //
    // http://blog.paulbetts.org/index.php/2010/07/20/the-case-of-the-disappearing-onload-exception-user-mode-callback-exceptions-in-x64/
    // https://support.microsoft.com/en-gb/kb/976038
    if(SetProcessUserModeExceptionPolicyPtr && GetProcessUserModeExceptionPolicyPtr)
    {
        DWORD flags = 0;

        if(GetProcessUserModeExceptionPolicyPtr(&flags))
            SetProcessUserModeExceptionPolicyPtr(flags & ~PROCESS_CALLBACK_FILTER_ENABLED);
    }
}

static void CrashDumpFatal(const char* Format, ...)
{
    char buffer[1024];
    va_list va;

    va_start(va, Format);
    vsnprintf_s(buffer, _TRUNCATE, Format, va);
    va_end(va);

    MessageBoxA(nullptr, buffer, "Error", MB_ICONERROR);
}

static void CrashDumpCreate(EXCEPTION_POINTERS* ExceptionPointers)
{
    // Generate a crash dump file in the minidump directory
    wchar_t dumpFile[MAX_PATH];
    wchar_t dumpDir[MAX_PATH];

    if(!GetModuleFileNameW(0,dumpDir,ARRAYSIZE(dumpDir)))
    {
        CrashDumpFatal("Unable to obtain process path during crash dump");
        return;
    }
    (wcsrchr(dumpDir,L'\\'))[0]=0;

    wcscat_s(dumpDir, L"\\minidump");
    CreateDirectoryW(dumpDir, nullptr);

    // Append the name with generated timestamp
    SYSTEMTIME st;
    GetLocalTime(&st);

    swprintf_s(dumpFile, L"%ws\\dump-%02d%02d%04d_%02d%02d%02d%04d.dmp", dumpDir,
               st.wDay,
               st.wMonth,
               st.wYear,
               st.wHour,
               st.wMinute,
               st.wSecond,
               st.wMilliseconds);

    // Open the file
    HANDLE fileHandle = CreateFileW(dumpFile, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if(fileHandle == INVALID_HANDLE_VALUE)
    {
        CrashDumpFatal("Failed to open file path '%ws' while generating crash dump", dumpFile);
        return;
    }

    // Create the minidump with DbgHelp
    MINIDUMP_EXCEPTION_INFORMATION info;
    memset(&info, 0, sizeof(MINIDUMP_EXCEPTION_INFORMATION));

    info.ThreadId = GetCurrentThreadId();
    info.ExceptionPointers = ExceptionPointers;
    info.ClientPointers = TRUE;

    if(!MiniDumpWriteDumpPtr(GetCurrentProcess(), GetCurrentProcessId(), fileHandle, kLargerDumpType, &info, nullptr, nullptr))
        CrashDumpFatal("MiniDumpWriteDump failed. Error: %u", GetLastError());

    // Close the file & done
    CloseHandle(fileHandle);
}

static LONG CALLBACK CrashDumpExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
    // Any "exception" under 0x1000 is usually a failed remote procedure call (RPC)
    if(ExceptionInfo && ExceptionInfo->ExceptionRecord->ExceptionCode > 0x1000)
    {
        switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
        {
        case DBG_PRINTEXCEPTION_C:  // OutputDebugStringA
        case 0x4001000A:            // OutputDebugStringW
        case STATUS_INVALID_HANDLE: // Invalid TitanEngine handle
        case 0x406D1388:            // SetThreadName
            return EXCEPTION_CONTINUE_SEARCH;
        }

        CrashDumpCreate(ExceptionInfo);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

static void InvalidParameterHandler(const wchar_t* Expression, const wchar_t* Function, const wchar_t* File, unsigned int Line, uintptr_t Reserved)
{
    CrashDumpFatal("Invalid parameter passed to CRT function! Program will now generate an exception.\n\nFile: %ws\nFunction: %ws\nExpression: %ws",
                   Function ? Function : L"???",
                   File ? File : L"???",
                   Expression ? Expression : L"???");

    // RaiseException: avoid the use of throw/C++ exceptions and possible __fastfail calls
    RaiseException(0x78646267, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}

static void TerminateHandler()
{
    CrashDumpFatal("Process termination was requested in an unusual way. Program will now generate an exception.");

    // See InvalidParameterHandler()
    RaiseException(0x78646267, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}

static void AbortHandler(int Signal)
{
    TerminateHandler();
}
