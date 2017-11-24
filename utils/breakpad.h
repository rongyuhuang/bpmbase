#pragma once

#if !defined(BPM_USE_BREAKPAD)

#include "utils/crashdump.h"

#else

#include <windows.h>

#include <DbgHelp.h>
#include <rpc.h>

#include <string>

// Minidump with stacks, PEB, TEB, and unloaded module list.
const MINIDUMP_TYPE kSmallDumpType = static_cast<MINIDUMP_TYPE>(
    MiniDumpWithProcessThreadData | // Get PEB and TEB.
    MiniDumpWithHandleData | MiniDumpWithUnloadedModules); // Get unloaded modules when available.

// Minidump with all of the above, plus memory referenced from stack.
const MINIDUMP_TYPE kLargerDumpType = static_cast<MINIDUMP_TYPE>(
    MiniDumpWithProcessThreadData | // Get PEB and TEB.
    MiniDumpWithHandleData | MiniDumpWithUnloadedModules | // Get unloaded modules when available.
    MiniDumpWithIndirectlyReferencedMemory); // Get memory referenced by stack.

// Large dump with all process memory.
const MINIDUMP_TYPE kFullDumpType = static_cast<MINIDUMP_TYPE>(
    MiniDumpWithFullMemory | // Full memory from process.
    MiniDumpWithProcessThreadData | // Get PEB and TEB.
    MiniDumpWithHandleData | // Get all handle information.
    MiniDumpWithUnloadedModules); // Get unloaded modules when available.

typedef BOOL(WINAPI* MiniDumpWriteDump_type)(
    HANDLE hProcess,
    DWORD dwPid,
    HANDLE hFile,
    MINIDUMP_TYPE DumpType,
    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

typedef RPC_STATUS(RPC_ENTRY* UuidCreate_type)(UUID* Uuid);

class Breakpad {
public:
    Breakpad();

    void init(const std::wstring& dump_path,
        MINIDUMP_TYPE dump_type = kLargerDumpType,
        bool handle_UnhandledException = true,
        bool handle_purecall = true,
        bool handle_invalid_parameter = true,
        bool consume_debug_exceptions = false,
        bool consume_invalid_handle_exceptions = false,
        bool consume_crt = false);
    void shutdown();

    bool WriteMinidump();

private:
    static DWORD WINAPI ExceptionHandlerThreadMain(void* lpParameter);
    static LONG WINAPI HandleException(EXCEPTION_POINTERS* exinfo);
    static void HandleInvalidParameter(const wchar_t* expression,
        const wchar_t* function,
        const wchar_t* file,
        unsigned int line,
        uintptr_t reserved);
    static void HandlePureVirtualCall();
    bool WriteMinidumpOnHandlerThread(EXCEPTION_POINTERS* exinfo);
    bool WriteMinidumpWithException(DWORD requesting_thread_id,
        EXCEPTION_POINTERS* exinfo);
    void UpdateNextID();
    std::wstring GUIDToWString(GUID* guid);

private:
    std::wstring dump_path_;
    std::wstring next_minidump_id_;
    std::wstring next_minidump_path_;

    const wchar_t* dump_path_c_ = nullptr;
    const wchar_t* next_minidump_id_c_ = nullptr;
    const wchar_t* next_minidump_path_c_ = nullptr;

    bool handle_UnhandledException_ = true;
    bool handle_purecall_ = true;
    bool handle_invalid_parameter_ = true;
    bool consume_debug_exceptions_ = false;
    bool consume_invalid_handle_exceptions_ = false;
    bool consume_crt_ = false;

    HMODULE dbghelp_module_ = nullptr;
    MiniDumpWriteDump_type minidump_write_dump_ = nullptr;
    MINIDUMP_TYPE dump_type_ = kLargerDumpType;

    HMODULE rpcrt4_module_ = nullptr;
    UuidCreate_type uuid_create_ = nullptr;

    LPTOP_LEVEL_EXCEPTION_FILTER previous_filter_ = nullptr;
    _invalid_parameter_handler previous_iph_ = nullptr;
    _purecall_handler previous_pch_ = nullptr;
    int previous_AssertMode_ = 0;
    int previous_ErrorMode_ = 0;

    HANDLE handler_thread_ = nullptr;
    volatile bool is_shutdown_ = false;
    CRITICAL_SECTION handler_critical_section_;
    HANDLE handler_start_semaphore_ = nullptr;
    HANDLE handler_finish_semaphore_ = nullptr;

    DWORD requesting_thread_id_ = 0;
    EXCEPTION_POINTERS* exception_info_ = nullptr;
    bool handler_return_value_ = false;
};

#endif // BPM_USE_BREAKPAD
