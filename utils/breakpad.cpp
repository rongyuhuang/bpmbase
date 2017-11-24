#include "breakpad.h"

#ifdef BPM_USE_BREAKPAD

#include <cassert>
#include <crtdbg.h>
#include <new.h>

static const int kWaitForHandlerThreadMs = 60000;
static const int kExceptionHandlerThreadInitialStackSize = 64 * 1024;

static Breakpad* g_breakPad_ = nullptr;

//////

Breakpad::Breakpad()
{
    g_breakPad_ = this;
}

void Breakpad::init(const std::wstring& dump_path,
    MINIDUMP_TYPE dump_type,
    bool handle_UnhandledException,
    bool handle_purecall,
    bool handle_invalid_parameter,
    bool consume_debug_exceptions,
    bool consume_invalid_handle_exceptions,
    bool consume_crt)
{
    InitializeCriticalSection(&handler_critical_section_);
    handler_start_semaphore_ = CreateSemaphore(NULL, 0, 1, NULL);
    assert(handler_start_semaphore_ != NULL);

    handler_finish_semaphore_ = CreateSemaphore(NULL, 0, 1, NULL);
    assert(handler_finish_semaphore_ != NULL);

    if (handler_finish_semaphore_ != NULL && handler_start_semaphore_ != NULL) {
        DWORD thread_id;
        handler_thread_ = CreateThread(NULL, // lpThreadAttributes
            kExceptionHandlerThreadInitialStackSize,
            ExceptionHandlerThreadMain,
            this, // lpParameter
            0, // dwCreationFlags
            &thread_id);
        assert(handler_thread_ != NULL);
    }

    dbghelp_module_ = LoadLibrary(L"dbghelp.dll");
    if (dbghelp_module_) {
        minidump_write_dump_ = reinterpret_cast<MiniDumpWriteDump_type>(
            GetProcAddress(dbghelp_module_, "MiniDumpWriteDump"));
    }

    rpcrt4_module_ = LoadLibrary(L"rpcrt4.dll");
    if (rpcrt4_module_) {
        uuid_create_ = reinterpret_cast<UuidCreate_type>(
            GetProcAddress(rpcrt4_module_, "UuidCreate"));
    }

    dump_path_ = dump_path;
    dump_path_c_ = dump_path_.c_str();
    UpdateNextID();
    dump_type_ = dump_type;

    handle_UnhandledException_ = handle_UnhandledException;
    handle_invalid_parameter_ = handle_invalid_parameter;
    handle_purecall_ = handle_purecall;
    consume_debug_exceptions_ = consume_debug_exceptions;
    consume_invalid_handle_exceptions_ = consume_invalid_handle_exceptions;
    consume_crt_ = consume_crt;

    if (handle_UnhandledException_) {
        previous_filter_ = SetUnhandledExceptionFilter(HandleException);
    }
    if (handle_invalid_parameter_) {
        previous_iph_ = _set_invalid_parameter_handler(HandleInvalidParameter);
    }
    if (handle_purecall_) {
        previous_pch_ = _set_purecall_handler(HandlePureVirtualCall);
    }
    if (!consume_crt_) {
        previous_AssertMode_ = _CrtSetReportMode(_CRT_ASSERT, 0);
        previous_ErrorMode_ = _CrtSetReportMode(_CRT_ERROR, 0);
    }
}

void Breakpad::shutdown()
{
    if (dbghelp_module_) {
        FreeLibrary(dbghelp_module_);
    }

    if (rpcrt4_module_) {
        FreeLibrary(rpcrt4_module_);
    }

    if (handle_UnhandledException_) {
        SetUnhandledExceptionFilter(previous_filter_);
    }
    if (handle_invalid_parameter_) {
        _set_invalid_parameter_handler(HandleInvalidParameter);
    }
    if (handle_purecall_) {
        _set_purecall_handler(previous_pch_);
    }
    if (!consume_crt_) {
        _CrtSetReportMode(_CRT_ASSERT, previous_AssertMode_);
        _CrtSetReportMode(_CRT_ERROR, previous_ErrorMode_);
    }

    if (0) {
        is_shutdown_ = true;
        ReleaseSemaphore(handler_start_semaphore_, 1, NULL);
        WaitForSingleObject(handler_thread_, kWaitForHandlerThreadMs);
    } else {
        TerminateThread(handler_thread_, 1);
    }

    CloseHandle(handler_thread_);
    handler_thread_ = NULL;
    DeleteCriticalSection(&handler_critical_section_);
    CloseHandle(handler_start_semaphore_);
    CloseHandle(handler_finish_semaphore_);
}

bool Breakpad::WriteMinidump()
{
    EXCEPTION_RECORD exception_record = {};
    CONTEXT exception_context = {};
    EXCEPTION_POINTERS exception_ptrs = { &exception_record, &exception_context };

    ::RtlCaptureContext(&exception_context);
    exception_record.ExceptionCode = STATUS_NONCONTINUABLE_EXCEPTION;

    return WriteMinidumpOnHandlerThread(&exception_ptrs);
}

// static
DWORD Breakpad::ExceptionHandlerThreadMain(void* lpParameter)
{
    Breakpad* self = reinterpret_cast<Breakpad*>(lpParameter);
    assert(self);
    assert(self->handler_start_semaphore_ != NULL);
    assert(self->handler_finish_semaphore_ != NULL);

    while (true) {
        if (WaitForSingleObject(self->handler_start_semaphore_, INFINITE) == WAIT_OBJECT_0) {
            if (self->is_shutdown_) {
                break;
            } else {
                self->handler_return_value_ = self->WriteMinidumpWithException(
                    self->requesting_thread_id_, self->exception_info_);
            }

            ReleaseSemaphore(self->handler_finish_semaphore_, 1, NULL);
        }
    }

    return 0;
}

// static
LONG WINAPI Breakpad::HandleException(EXCEPTION_POINTERS* exinfo)
{
    DWORD code = exinfo->ExceptionRecord->ExceptionCode;
    LONG action;
    bool is_debug_exception = (code == EXCEPTION_BREAKPOINT) || (code == EXCEPTION_SINGLE_STEP);

    if (code == EXCEPTION_INVALID_HANDLE && g_breakPad_->consume_invalid_handle_exceptions_) {
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    bool success = false;

    if (!is_debug_exception || !g_breakPad_->consume_debug_exceptions_) {
        success = g_breakPad_->WriteMinidumpOnHandlerThread(exinfo);
    }

    if (success) {
        action = EXCEPTION_EXECUTE_HANDLER;
    } else {
        if (g_breakPad_->previous_filter_) {
            action = g_breakPad_->previous_filter_(exinfo);
        } else {
            action = EXCEPTION_CONTINUE_SEARCH;
        }
    }

    return action;
}

// static
void Breakpad::HandleInvalidParameter(const wchar_t* expression,
    const wchar_t* function,
    const wchar_t* file,
    unsigned int line,
    uintptr_t reserved)
{
    EXCEPTION_RECORD exception_record = {};
    CONTEXT exception_context = {};
    EXCEPTION_POINTERS exception_ptrs = { &exception_record, &exception_context };

    ::RtlCaptureContext(&exception_context);

    exception_record.ExceptionCode = STATUS_INVALID_PARAMETER;
    exception_record.NumberParameters = 0;

    bool success = false;
    success = g_breakPad_->WriteMinidumpOnHandlerThread(&exception_ptrs);

    if (!success) {
        if (g_breakPad_->previous_iph_) {
            g_breakPad_->previous_iph_(expression, function, file, line, reserved);
        } else {
#ifdef _DEBUG
            _invalid_parameter(expression, function, file, line, reserved);
#else // _DEBUG
            _invalid_parameter_noinfo();
#endif // _DEBUG
        }
    }

    exit(0);
}

// static
void Breakpad::HandlePureVirtualCall()
{
    EXCEPTION_RECORD exception_record = {};
    CONTEXT exception_context = {};
    EXCEPTION_POINTERS exception_ptrs = { &exception_record, &exception_context };

    ::RtlCaptureContext(&exception_context);

    exception_record.ExceptionCode = STATUS_NONCONTINUABLE_EXCEPTION;

    exception_record.NumberParameters = 0;

    bool success = false;
    success = g_breakPad_->WriteMinidumpOnHandlerThread(&exception_ptrs);

    if (!success) {
        if (g_breakPad_->previous_pch_) {
            g_breakPad_->previous_pch_();
        } else {
            return;
        }
    }

    exit(0);
}

bool Breakpad::WriteMinidumpOnHandlerThread(EXCEPTION_POINTERS* exinfo)
{
    EnterCriticalSection(&handler_critical_section_);

    if (handler_thread_ == NULL) {
        LeaveCriticalSection(&handler_critical_section_);
        return false;
    }

    assert(handler_start_semaphore_ != NULL);
    assert(handler_finish_semaphore_ != NULL);

    requesting_thread_id_ = GetCurrentThreadId();
    exception_info_ = exinfo;

    ReleaseSemaphore(handler_start_semaphore_, 1, NULL);

    WaitForSingleObject(handler_finish_semaphore_, INFINITE);
    bool status = handler_return_value_;

    requesting_thread_id_ = 0;
    exception_info_ = NULL;

    UpdateNextID();

    LeaveCriticalSection(&handler_critical_section_);

    return status;
}

bool Breakpad::WriteMinidumpWithException(DWORD requesting_thread_id,
    EXCEPTION_POINTERS* exinfo)
{
    bool success = false;
    if (minidump_write_dump_) {
        HANDLE dump_file = CreateFile(next_minidump_path_c_, GENERIC_WRITE,
            0, // no sharing
            NULL,
            CREATE_NEW, // fail if exists
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (dump_file != INVALID_HANDLE_VALUE) {
            MINIDUMP_EXCEPTION_INFORMATION except_info;
            except_info.ThreadId = requesting_thread_id;
            except_info.ExceptionPointers = exinfo;
            except_info.ClientPointers = FALSE;

            success = (minidump_write_dump_(GetCurrentProcess(),
                           GetCurrentProcessId(), dump_file,
                           dump_type_, exinfo ? &except_info : NULL,
                           nullptr, nullptr)
                == TRUE);

            CloseHandle(dump_file);
        }
    }

    return success;
}

void Breakpad::UpdateNextID()
{
    assert(uuid_create_);
    UUID id = { 0 };
    if (uuid_create_) {
        uuid_create_(&id);
    }
    next_minidump_id_ = GUIDToWString(&id);
    next_minidump_id_c_ = next_minidump_id_.c_str();

    wchar_t minidump_path[MAX_PATH];
    swprintf(minidump_path, MAX_PATH, L"%s\\%s.dmp", dump_path_c_,
        next_minidump_id_c_);

    next_minidump_path_ = minidump_path;
    next_minidump_path_c_ = next_minidump_path_.c_str();
}

std::wstring Breakpad::GUIDToWString(GUID* guid)
{
    wchar_t guid_string[37];
    swprintf(guid_string, sizeof(guid_string) / sizeof(guid_string[0]),
        L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", guid->Data1,
        guid->Data2, guid->Data3, guid->Data4[0], guid->Data4[1],
        guid->Data4[2], guid->Data4[3], guid->Data4[4], guid->Data4[5],
        guid->Data4[6], guid->Data4[7]);

    return std::wstring(guid_string);
}

//////
namespace {

#pragma optimize("", off)
void InvalidParameter(const wchar_t* expression,
    const wchar_t* function,
    const wchar_t* file,
    unsigned int line,
    uintptr_t reserved)
{
    __debugbreak();
    _exit(1);
}

void PureCall()
{
    __debugbreak();
    _exit(1);
}
#pragma optimize("", on)
}

void BreakpadStaticLib::RegisterInvalidParamHandler()
{
    _set_invalid_parameter_handler(InvalidParameter);
    _set_purecall_handler(PureCall);
    _set_new_mode(1);
}

void BreakpadStaticLib::SetupCRT()
{
    _CrtSetReportMode(_CRT_ASSERT, 0);
    _CrtSetReportMode(_CRT_ERROR, 0);
}

#endif // BPM_USE_BREAKPAD
