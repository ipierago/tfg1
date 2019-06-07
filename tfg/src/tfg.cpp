#include "stdafx.h"
#include "tfg.h"

typedef struct TFG_ThreadGlobals
{
    int32_t line_number;
    const char *function_psz;
    TFG_Level std_level;
} TFG_ThreadGlobals;

struct TFG_Globals
{
    std::mutex mutex;
    std::vector<TFG_PerFileData *> per_file_data_std_ptrvec;
    std::vector<TFG_ThreadGlobals *> TFG_ThreadGlobals_std_ptrvec;
    TFG_Level std_level_global;

    TFG_Globals();
    ~TFG_Globals();
};

TFG_Globals &TFG_Globals_get()
{
    static TFG_Globals s_TFG_Globals;
    return s_TFG_Globals;
}

TFG_ThreadGlobals *TFG_ThreadGlobals_create();
void TFG_ThreadGlobals_destroy(TFG_ThreadGlobals *const in_this);

TFG_Globals::TFG_Globals()
{
#ifdef _DEBUG
	std_level_global = TFG_Level_Debug;
#else
	std_level_global = TFG_Level_Warning;
#endif
}

TFG_Globals::~TFG_Globals()
{
    std::unique_lock<std::mutex> _unique_lock(mutex);
    auto const size_per_file_data_std_ptrvec = per_file_data_std_ptrvec.size();
    for (auto i = 0; i < size_per_file_data_std_ptrvec; ++i)
    {
        TFG_PerFileData *const std_per_file_data_p = per_file_data_std_ptrvec.at(i);
        TFG_PerFileData_Deinit(std_per_file_data_p);
    }
    auto const size_thread_globals_std_ptrvec = TFG_ThreadGlobals_std_ptrvec.size();
    for (auto i = 0; i < size_thread_globals_std_ptrvec; ++i)
    {
        TFG_ThreadGlobals *const TFG_ThreadGlobals_p = TFG_ThreadGlobals_std_ptrvec.at(i);
        TFG_ThreadGlobals_destroy(TFG_ThreadGlobals_p);
    }
}

void TFG_Init()
{
    TFG_Globals &globals = TFG_Globals_get();
}

void TFG_Deinit()
{
}

void TFG_SetGlobalLevelLog(TFG_Level const in_level)
{
    TFG_Globals &globals = TFG_Globals_get();
    std::unique_lock<std::mutex> _unique_lock(globals.mutex);
    globals.std_level_global = in_level;
    auto const size = globals.per_file_data_std_ptrvec.size();
    for (auto i = 0; i < size; ++i)
    {
        TFG_PerFileDataPtr const std_per_file_data_p = globals.per_file_data_std_ptrvec.at(i);
        std_per_file_data_p->current_std_level = std::max(std_per_file_data_p->file_std_level, globals.std_level_global);
    }
}

void TFG_PerFileData_Init(TFG_PerFileDataPtr const in_this, const char *const in_filename_psz, TFG_Level const in_std_level)
{
    TFG_Globals &globals = TFG_Globals_get();
    in_this->initialized = 1;
    in_this->filename_psz = in_filename_psz;
    in_this->current_std_level = std::max(in_std_level, globals.std_level_global);
    in_this->file_std_level = in_std_level;
    std::unique_lock<std::mutex> _unique_lock(globals.mutex);
    globals.per_file_data_std_ptrvec.push_back(in_this);
}

void TFG_PerFileData_Deinit(TFG_PerFileDataPtr const in_this)
{
    // TODO: Remove from vec?
    UNREFERENCED_PARAMETER(in_this);
}

TFG_ThreadGlobals *TFG_ThreadGlobals_create()
{
    TFG_Globals &globals = TFG_Globals_get();
    TFG_ThreadGlobals *const TFG_ThreadGlobals_p = new TFG_ThreadGlobals;
    memset(TFG_ThreadGlobals_p, 0, sizeof(TFG_ThreadGlobals));
    std::unique_lock<std::mutex> _unique_lock(globals.mutex);
    globals.TFG_ThreadGlobals_std_ptrvec.push_back(TFG_ThreadGlobals_p);
    return TFG_ThreadGlobals_p;
}

void TFG_ThreadGlobals_destroy(TFG_ThreadGlobals *const in_this)
{
    TFG_Globals &globals = TFG_Globals_get();
    delete in_this;
}

TFG_ThreadGlobals *TFG_ThreadGlobals_Get()
{
    static THREAD_LOCAL_STORAGE_DECLSPEC TFG_ThreadGlobals *TFG_ThreadGlobals_p = 0;
    if (TFG_ThreadGlobals_p == 0)
    {
        TFG_ThreadGlobals_p = TFG_ThreadGlobals_create();
    }
    return TFG_ThreadGlobals_p;
}

void TFG_LogInit(const int32_t in_line_number, const char *const in_function_psz, TFG_Level const in_std_level)
{
    TFG_ThreadGlobals *const TFG_ThreadGlobals_p = TFG_ThreadGlobals_Get();
    TFG_ThreadGlobals_p->line_number = in_line_number;
    TFG_ThreadGlobals_p->function_psz = in_function_psz;
    TFG_ThreadGlobals_p->std_level = in_std_level;
}

#ifdef ANDROID
static android_LogPriority ConvertToAndroidLogPriority(TFG_Level const in_level)
{
    if (in_level == TFG_Level_Always)
        return ANDROID_LOG_UNKNOWN;
    if (in_level == TFG_Level_Severe)
        return ANDROID_LOG_FATAL;
    if (in_level == TFG_Level_Error)
        return ANDROID_LOG_ERROR;
    if (in_level == TFG_Level_Warning)
        return ANDROID_LOG_WARN;
    if (in_level == TFG_Level_Notify)
        return ANDROID_LOG_INFO;
    if (in_level == TFG_Level_Info)
        return ANDROID_LOG_INFO;
    if (in_level == TFG_Level_Debug)
        return ANDROID_LOG_DEBUG;
    if (in_level == TFG_Level_Debug2)
        return ANDROID_LOG_DEBUG;
    if (in_level == TFG_Level_Trace)
        return ANDROID_LOG_VERBOSE;
    if (in_level == TFG_Level_Trace2)
        return ANDROID_LOG_VERBOSE;
    if (in_level == TFG_Level_Never)
        return ANDROID_LOG_SILENT;
    return ANDROID_LOG_UNKNOWN;
}
#endif
static void _log_str(const char *const in_)
{
#if defined(ANDROID)
    TFG_ThreadGlobals *const TFG_ThreadGlobals_p = TFG_ThreadGlobals_Get();
    android_LogPriority const logPriority = ConvertToAndroidLogPriority(TFG_ThreadGlobals_p->std_level);
    __android_log_write(logPriority, TFG_ThreadGlobals_p->function_psz, in_);
#elif defined(WIN32)
    OutputDebugStringA(in_);
//#elif defined(__APPLE__)
#endif
    printf("%s", in_);
}

inline int32_t TFG_vsprintf_s(char *const in_buffer, size_t const in_buffer_count, const char *const format, va_list argList)
{
#if defined(_MSC_VER)
    return vsprintf_s(in_buffer, in_buffer_count, format, argList);
#elif defined(__GNUC__)
    return vsprintf(in_buffer, format, argList);
#endif
}

inline int32_t TFG_sprintf_s(char *const in_buffer, size_t const in_buffer_count, const char *const format, ...)
{
    va_list argList;
    va_start(argList, format);
    int32_t const rv = TFG_vsprintf_s(in_buffer, in_buffer_count, format, argList);
    va_end(argList);
    return rv;
}

inline int TFG_strcat_s(char *dest, size_t destsz, const char *src)
{
#if defined(_MSC_VER)
    return strcat_s(dest, destsz, src);
#elif defined(__GNUC__)
    strcat(dest, src);
    return 0;
#endif
}

inline uint32_t TFG_GetCurrentThreadId()
{
#if defined(_WIN32)
    return GetCurrentThreadId();
#elif defined(ANDROID)
    return gettid();
#else
    mach_port_t machTID = pthread_mach_thread_np(pthread_self());
    return machTID;
#endif
}

static int32_t sprintf_header(char *const in_buffer, size_t const in_buffer_count)
{
    uint32_t this_thread_id = TFG_GetCurrentThreadId();
    TFG_ThreadGlobals *const TFG_ThreadGlobals_p = TFG_ThreadGlobals_Get();
    int32_t const rv = TFG_sprintf_s(in_buffer, in_buffer_count, "T0x%08lx (%s) %s: ", this_thread_id, TFG_LevelToStringShort(TFG_ThreadGlobals_p->std_level), TFG_ThreadGlobals_p->function_psz);
    return rv;
}

static void _log_vargs(const char *const format, va_list argList)
{
    char buf[1024];
    int32_t const headerLen = sprintf_header(buf, sizeof(buf));
    TFG_vsprintf_s(buf + headerLen, sizeof(buf) - headerLen, format, argList);
    TFG_strcat_s(buf, sizeof(buf), "\n");
    _log_str(buf);
}

void TFG_Log(const char *const format, ...)
{
    va_list argList;
    va_start(argList, format);
    _log_vargs(format, argList);
    va_end(argList);
}

void TFG_FuncEnter()
{
    char buf[1024];
    int32_t len = 0;
    len += sprintf_header(buf, sizeof(buf));
    len += TFG_sprintf_s(buf + len, sizeof(buf) - len, "(enter) \n");
    _log_str(buf);
}

void TFG_FuncExit(const char *const in_FormatParameters, ...)
{
    va_list argList;
    va_start(argList, in_FormatParameters);
    char buf[1024];
    int32_t len = 0;
    len += sprintf_header(buf, sizeof(buf));
    len += TFG_sprintf_s(buf + len, sizeof(buf) - len, "(exit ) ");
    len += TFG_vsprintf_s(buf + len, sizeof(buf) - len, in_FormatParameters, argList);
    len += TFG_strcat_s(buf + len, sizeof(buf) - len, "\n");
    va_end(argList);
    _log_str(buf);
}

const char *TFG_LevelToStringShort(TFG_Level const in_level)
{
    switch (in_level)
    {
    case TFG_Level_Always:
        return "always ";
    case TFG_Level_Severe:
        return "severe ";
    case TFG_Level_Error:
        return "error  ";
    case TFG_Level_Warning:
        return "warning";
    case TFG_Level_Notify:
        return "notify ";
    case TFG_Level_Info:
        return "info   ";
    case TFG_Level_Debug:
        return "debug  ";
    case TFG_Level_Debug2:
        return "debug_2";
    case TFG_Level_Trace:
        return "trace  ";
    case TFG_Level_Trace2:
        return "trace_2";
    default:
        return "unknown";
    }
}
