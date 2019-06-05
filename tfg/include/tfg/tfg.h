#pragma once

#include "TFG_fwd.h"
#include "djb2.h"
#include "error.h"
#include "log.h"

//#define TFG_DISABLE
//#define TFG_ASSERT_DISABLE
//#define TFG_DPF_ENTER_EXIT_FUNC_DISABLE

#define PRIxPTR_WIDTH ((int32_t)(sizeof(uintptr_t) * 2))

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum TFG_Level
    {
        TFG_Level_Always,

        TFG_Level_Severe,
        TFG_Level_Error,
        TFG_Level_Warning,

        TFG_Level_Notify,
        TFG_Level_Info,

        TFG_Level_Debug,
        TFG_Level_Debug2,
        TFG_Level_Trace,
        TFG_Level_Trace2,

        TFG_Level_Never
    } TFG_Level;

    const char *TFG_LevelToStringShort(TFG_Level const in_level);

    void TFG_Init();
    void TFG_Deinit();

    // Set minimum values across files
    void TFG_SetGlobalLevelLog(TFG_Level const in_level);

    // TODO: thread data, or Use push/pop on stack instead?
    void TFG_LogInit(const int32_t in_line_number, const char *const in_function_psz, TFG_Level const in_std_level);
    void TFG_Log(const char *const, ...);
    void TFG_FuncExit(const char *const, ...);
    void TFG_FuncEnter();

    typedef struct TFG_PerFileData
    {
        int32_t initialized;
        const char *filename_psz;
        TFG_Level current_std_level;
        TFG_Level file_std_level;
    } TFG_PerFileData;
    typedef TFG_PerFileData *TFG_PerFileDataPtr;

    void TFG_PerFileData_Init(TFG_PerFileDataPtr const in_this, const char *const in_filename_psz, TFG_Level const in_std_level);
    void TFG_PerFileData_Deinit(TFG_PerFileDataPtr const in_this);

#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#define FORCE_INLINE_DECLSPEC __forceinline
#define DEBUG_BREAK_FUNCTION DebugBreak
#endif
#ifdef __GNUC__
#define FORCE_INLINE_DECLSPEC __attribute__((always_inline))
#define DEBUG_BREAK_FUNCTION __builtin_trap
#endif

#ifndef TFG_DISABLE
#define TFG_FILE_SETUP_EX(IN_FILE_TFG_DPF_LEVEL)                                       \
    static TFG_PerFileData s_std_perFileData;                                          \
    static FORCE_INLINE_DECLSPEC TFG_Level TFG_PerFileData_GetCurrentLevel()           \
    {                                                                                  \
        if (s_std_perFileData.initialized == 0)                                        \
            TFG_PerFileData_Init(&s_std_perFileData, __FILE__, IN_FILE_TFG_DPF_LEVEL); \
        return s_std_perFileData.current_std_level;                                    \
    }
#define TFG_FILE_SETUP() TFG_FILE_SETUP_EX(TFG_Level_Warning)

#define TFG_CHECK_LEVEL(_level) (TFG_PerFileData_GetCurrentLevel() >= _level)
#define TFG_GOTO_FINALLY(_LEVEL)          \
    TFG_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL); \
    goto finally;

#ifndef TFG_ASSERT_DISABLE
#define TFG_ASSERT(_EXPRESSION)                        \
    if (!(_EXPRESSION))                                \
    {                                                  \
        TFG_SEVERE("%s", #_EXPRESSION);                \
        TFG_SET_THIS_LEVEL_FUNC_EXIT(TFG_Level_Error); \
        DEBUG_BREAK_FUNCTION();                        \
    }
#else
#define TFG_ASSERT(_EXPRESSION) __noop
#endif

#define TFG_CHECK(_EXPRESSION)                         \
    if (!(_EXPRESSION))                                \
    {                                                  \
        TFG_ERROR("%s", #_EXPRESSION);                 \
        TFG_SET_THIS_LEVEL_FUNC_EXIT(TFG_Level_Error); \
        goto finally;                                  \
    }
#define TFG_CHECK_NO_GOTO(_EXPRESSION)                 \
    if (!(_EXPRESSION))                                \
    {                                                  \
        TFG_ERROR("%s", #_EXPRESSION);                 \
        TFG_SET_THIS_LEVEL_FUNC_EXIT(TFG_Level_Error); \
    }
#define TFG_BOOL_TO_STRING(_BOOL) (_BOOL ? "TRUE" : "FALSE")

#define TFG_SEVERE                         \
    if (TFG_CHECK_LEVEL(TFG_Level_Severe)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Severe), TFG_Log
#define TFG_ERROR                         \
    if (TFG_CHECK_LEVEL(TFG_Level_Error)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Error), TFG_Log
#define TFG_WARNING                         \
    if (TFG_CHECK_LEVEL(TFG_Level_Warning)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Warning), TFG_Log
#define TFG_NOTIFY                         \
    if (TFG_CHECK_LEVEL(TFG_Level_Notify)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Notify), TFG_Log
#define TFG_INFO                         \
    if (TFG_CHECK_LEVEL(TFG_Level_Info)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Info), TFG_Log
#define TFG_DEBUG                         \
    if (TFG_CHECK_LEVEL(TFG_Level_Debug)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Debug), TFG_Log
#define TFG_DEBUG_2                        \
    if (TFG_CHECK_LEVEL(TFG_Level_Debug2)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Debug2), TFG_Log
#define TFG_TRACE                         \
    if (TFG_CHECK_LEVEL(TFG_Level_Trace)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Trace), TFG_Log
#define TFG_TRACE_2                        \
    if (TFG_CHECK_LEVEL(TFG_Level_Trace2)) \
    TFG_LogInit(__LINE__, __FUNCTION__, TFG_Level_Trace2), TFG_Log

#ifndef TFG_DPF_ENTER_EXIT_FUNC_DISABLE
#define TFG_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL) s_CurrentLevelFuncEnterExit = _LEVEL
#define TFG_FUNC_ENTER                                        \
    TFG_Level s_CurrentLevelFuncEnterExit = TFG_Level_Trace2; \
    if (TFG_CHECK_LEVEL(s_CurrentLevelFuncEnterExit))         \
    TFG_LogInit(__LINE__, __FUNCTION__, s_CurrentLevelFuncEnterExit), TFG_FuncEnter
#define TFG_FUNC_EXIT                                 \
    if (TFG_CHECK_LEVEL(s_CurrentLevelFuncEnterExit)) \
    TFG_LogInit(__LINE__, __FUNCTION__, s_CurrentLevelFuncEnterExit), TFG_FuncExit
#else
#define TFG_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL)
#define TFG_FUNC_ENTER __noop
#define TFG_FUNC_EXIT __noop
#endif

#else
#define TFG_FILE_SETUP_EX(_IN)
#define TFG_FILE_SETUP()
#define TFG_CHECK_LEVEL(_level)
#define TFG_GOTO_FINALLY(_LEVEL) goto finally;
#define TFG_ASSERT(_EXPRESSION)
#define TFG_CHECK(_EXPRESSION) \
    if (!(_EXPRESSION))        \
        goto finally;
#define TFG_CHECK_NO_GOTO(_EXPRESSION) (_EXPRESSION)
#define TFG_BOOL_TO_STRING(_BOOL) (_BOOL ? "TRUE" : "FALSE")

#define TFG_SEVERE __noop
#define TFG_ERROR __noop
#define TFG_WARNING __noop
#define TFG_NOTIFY __noop
#define TFG_INFO __noop
#define TFG_DEBUG __noop
#define TFG_DEBUG_2 __noop
#define TFG_TRACE __noop
#define TFG_TRACE_2 __noop

#define TFG_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL)
#define TFG_FUNC_ENTER __noop
#define TFG_FUNC_EXIT __noop

#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    void operator=(const TypeName &)


