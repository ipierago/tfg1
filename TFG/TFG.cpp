#include "TFG.h"

typedef struct std_dpf_thread_globals_t
{
    int32_t line_number;
    const char *function_psz;
    StdLevel std_level;
} std_dpf_thread_globals_t;

struct std_dpf_globals_t
{
    std::mutex mutex;
    std::vector<Std_PerFileData *> per_file_data_std_ptrvec;
    std::vector<std_dpf_thread_globals_t *> std_dpf_thread_globals_std_ptrvec;
    StdLevel std_level_global;

    std_dpf_globals_t();
    ~std_dpf_globals_t();
};
typedef std_dpf_globals_t *std_dpf_globals_p_t;

std_dpf_globals_t &std_dpf_globals_t_get()
{
    static std_dpf_globals_t s_std_dpf_globals;
    return s_std_dpf_globals;
}

typedef std_dpf_thread_globals_t *std_dpf_thread_globals_p_t;
std_dpf_thread_globals_p_t std_dpf_thread_globals_create();
void std_dpf_thread_globals_destroy(std_dpf_thread_globals_p_t const in_this);

std_dpf_globals_t::std_dpf_globals_t()
{
    std_level_global = StdLevel_Warning;
}

std_dpf_globals_t::~std_dpf_globals_t()
{
    {
        std::unique_lock<std::mutex> _unique_lock(mutex);
        uint32_t const size_per_file_data_std_ptrvec = per_file_data_std_ptrvec.size();
        for (uint32_t i = 0; i < size_per_file_data_std_ptrvec; ++i)
        {
            Std_PerFileData *const std_per_file_data_p = per_file_data_std_ptrvec.at(i);
            Std_PerFileData_Deinit(std_per_file_data_p);
        }
        uint32_t const size_thread_globals_std_ptrvec = std_dpf_thread_globals_std_ptrvec.size();
        for (uint32_t i = 0; i < size_thread_globals_std_ptrvec; ++i)
        {
            std_dpf_thread_globals_p_t const std_dpf_thread_globals_p = std_dpf_thread_globals_std_ptrvec.at(i);
            std_dpf_thread_globals_destroy(std_dpf_thread_globals_p);
        }
    }
}

void Std_Init()
{
}

void Std_Deinit()
{
}

void Std_SetGlobalLevelLog(StdLevel const in_level)
{
    std_dpf_globals_t &g_std_dpf_globals = std_dpf_globals_t_get();
    std::unique_lock<std::mutex> _unique_lock(g_std_dpf_globals.mutex);
    g_std_dpf_globals.std_level_global = in_level;
    uint32_t const size = g_std_dpf_globals.per_file_data_std_ptrvec.size();
    for (uint32_t i = 0; i < size; ++i)
    {
        Std_PerFileDataPtr const std_per_file_data_p = g_std_dpf_globals.per_file_data_std_ptrvec.at(i);
        std_per_file_data_p->current_std_level = std::max(std_per_file_data_p->file_std_level, g_std_dpf_globals.std_level_global);
    }
}

void Std_PerFileData_Init(Std_PerFileDataPtr const in_this, const char *const in_filename_psz, StdLevel const in_std_level)
{
    std_dpf_globals_t &g_std_dpf_globals = std_dpf_globals_t_get();
    in_this->initialized = 1;
    in_this->filename_psz = in_filename_psz;
    in_this->current_std_level = std::max(in_std_level, g_std_dpf_globals.std_level_global);
    in_this->file_std_level = in_std_level;
    std::unique_lock<std::mutex> _unique_lock(g_std_dpf_globals.mutex);
    g_std_dpf_globals.per_file_data_std_ptrvec.push_back(in_this);
}

void Std_PerFileData_Deinit(Std_PerFileDataPtr const in_this)
{
    // TODO: Remove from vec?
    UNREFERENCED_PARAMETER(in_this);
}

std_dpf_thread_globals_p_t std_dpf_thread_globals_create()
{
    std_dpf_globals_t &g_std_dpf_globals = std_dpf_globals_t_get();
    std_dpf_thread_globals_p_t const std_dpf_thread_globals_p = new std_dpf_thread_globals_t;
    memset(std_dpf_thread_globals_p, 0, sizeof(std_dpf_thread_globals_t));
    std::unique_lock<std::mutex> _unique_lock(g_std_dpf_globals.mutex);
    g_std_dpf_globals.std_dpf_thread_globals_std_ptrvec.push_back(std_dpf_thread_globals_p);
    return std_dpf_thread_globals_p;
}

void std_dpf_thread_globals_destroy(std_dpf_thread_globals_p_t const in_this)
{
    std_dpf_globals_t &g_std_dpf_globals = std_dpf_globals_t_get();
    delete in_this;
}

std_dpf_thread_globals_p_t get_std_dpf_thread_globals()
{
    static THREAD_LOCAL_STORAGE_DECLSPEC std_dpf_thread_globals_p_t std_dpf_thread_globals_p = 0;
    if (std_dpf_thread_globals_p == 0)
    {
        std_dpf_thread_globals_p = std_dpf_thread_globals_create();
    }
    return std_dpf_thread_globals_p;
}

void Std_LogInit(const int32_t in_line_number, const char *const in_function_psz, StdLevel const in_std_level)
{
    std_dpf_thread_globals_p_t const std_dpf_thread_globals_p = get_std_dpf_thread_globals();
    std_dpf_thread_globals_p->line_number = in_line_number;
    std_dpf_thread_globals_p->function_psz = in_function_psz;
    std_dpf_thread_globals_p->std_level = in_std_level;
}

#ifdef ANDROID
static android_LogPriority ConvertToAndroidLogPriority(StdLevel const in_level)
{
    if (in_level == StdLevel_Always)
        return ANDROID_LOG_UNKNOWN;
    if (in_level == StdLevel_Severe)
        return ANDROID_LOG_FATAL;
    if (in_level == StdLevel_Error)
        return ANDROID_LOG_ERROR;
    if (in_level == StdLevel_Warning)
        return ANDROID_LOG_WARN;
    if (in_level == StdLevel_Notify)
        return ANDROID_LOG_INFO;
    if (in_level == StdLevel_Info)
        return ANDROID_LOG_INFO;
    if (in_level == StdLevel_Debug)
        return ANDROID_LOG_DEBUG;
    if (in_level == StdLevel_Debug2)
        return ANDROID_LOG_DEBUG;
    if (in_level == StdLevel_Trace)
        return ANDROID_LOG_VERBOSE;
    if (in_level == StdLevel_Trace2)
        return ANDROID_LOG_VERBOSE;
    if (in_level == StdLevel_Never)
        return ANDROID_LOG_SILENT;
    return ANDROID_LOG_UNKNOWN;
}
#endif
static void _log_str(const char *const in_)
{
#if defined(ANDROID)
    std_dpf_thread_globals_p_t const std_dpf_thread_globals_p = get_std_dpf_thread_globals();
    android_LogPriority const logPriority = ConvertToAndroidLogPriority(std_dpf_thread_globals_p->std_level);
    __android_log_write(logPriority, std_dpf_thread_globals_p->function_psz, in_);
#elif defined(WIN32)
    OutputDebugStringA(in_);
//#elif defined(__APPLE__)
#endif
    printf("%s\n", in_);
}

inline int32_t Std_vsprintf_s(char *const in_buffer, size_t const in_buffer_count, const char *const format, va_list argList)
{
#if defined(_MSC_VER)
    return vsprintf_s(in_buffer, in_buffer_count, format, argList);
#elif defined(__GNUC__)
    return vsprintf(in_buffer, format, argList);
#endif
}

inline int32_t Std_sprintf_s(char *const in_buffer, size_t const in_buffer_count, const char *const format, ...)
{
    va_list argList;
    va_start(argList, format);
    int32_t const rv = Std_vsprintf_s(in_buffer, in_buffer_count, format, argList);
    va_end(argList);
    return rv;
}

inline int Std_strcat_s(char *dest, size_t destsz, const char *src)
{
#if defined(_MSC_VER)
    return strcat_s(dest, destsz, src);
#elif defined(__GNUC__)
    strcat(dest, src);
    return 0;
#endif
}

inline uint32_t Std_GetCurrentThreadId()
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
    uint32_t this_thread_id = Std_GetCurrentThreadId();
    std_dpf_thread_globals_p_t const std_dpf_thread_globals_p = get_std_dpf_thread_globals();
    int32_t const rv = Std_sprintf_s(in_buffer, in_buffer_count, "T0x%08lx (%s) %s: ", this_thread_id, Std_LevelToStringShort(std_dpf_thread_globals_p->std_level), std_dpf_thread_globals_p->function_psz);
    return rv;
}

static void _log_vargs(const char *const format, va_list argList)
{
    char buf[1024];
    int32_t const headerLen = sprintf_header(buf, sizeof(buf));
    Std_vsprintf_s(buf + headerLen, sizeof(buf) - headerLen, format, argList);
    Std_strcat_s(buf, sizeof(buf), "\n");
    _log_str(buf);
}

void Std_Log(const char *const format, ...)
{
    va_list argList;
    va_start(argList, format);
    _log_vargs(format, argList);
    va_end(argList);
}

void Std_FuncEnter()
{
    char buf[1024];
    int32_t len = 0;
    len += sprintf_header(buf, sizeof(buf));
    len += Std_sprintf_s(buf + len, sizeof(buf) - len, "(enter) ");
    _log_str(buf);
}

void Std_FuncExit(const char *const in_FormatParameters, ...)
{
    va_list argList;
    va_start(argList, in_FormatParameters);
    char buf[1024];
    int32_t len = 0;
    len += sprintf_header(buf, sizeof(buf));
    len += Std_sprintf_s(buf + len, sizeof(buf) - len, "(exit ) ");
    len += Std_vsprintf_s(buf + len, sizeof(buf) - len, in_FormatParameters, argList);
    len += Std_strcat_s(buf + len, sizeof(buf) - len, "\n");
    va_end(argList);
    _log_str(buf);
}

const char *Std_LevelToStringShort(StdLevel const in_level)
{
    switch (in_level)
    {
    case StdLevel_Always:
        return "always ";
    case StdLevel_Severe:
        return "severe ";
    case StdLevel_Error:
        return "error  ";
    case StdLevel_Warning:
        return "warning";
    case StdLevel_Notify:
        return "notify ";
    case StdLevel_Info:
        return "info   ";
    case StdLevel_Debug:
        return "debug  ";
    case StdLevel_Debug2:
        return "debug_2";
    case StdLevel_Trace:
        return "trace  ";
    case StdLevel_Trace2:
        return "trace_2";
    default:
        return "unknown";
    }
}

namespace TFG
{

uint32_t TFG::s_Param0;

TFG::TFG(uint32_t const in_Param0)
{
    s_Param0 = in_Param0;
}

TFG::~TFG()
{
}

} // namespace TFG
