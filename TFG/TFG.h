#pragma once

#include "TFG_fwd.h"
#include "djb2.h"
#include "error.h"

//#define STD_DISABLE
//#define STD_ASSERT_DISABLE
//#define STD_DPF_ENTER_EXIT_FUNC_DISABLE

#define PRIxPTR_WIDTH ((int32_t)(sizeof(uintptr_t) * 2))

typedef enum StdLevel
{
    StdLevel_Always,

    StdLevel_Severe,
    StdLevel_Error,
    StdLevel_Warning,

    StdLevel_Notify,
    StdLevel_Info,

    StdLevel_Debug,
    StdLevel_Debug2,
    StdLevel_Trace,
    StdLevel_Trace2,

    StdLevel_Never
} StdLevel;

const char *Std_LevelToStringShort(StdLevel const in_level);

void Std_Init();
void Std_Deinit();

// Set minimum values across files
void Std_SetGlobalLevelLog(StdLevel const in_level);

// TODO: thread data, or Use push/pop on stack instead?
void Std_LogInit(const int32_t in_line_number, const char *const in_function_psz, StdLevel const in_std_level);
void Std_Log(const char *const, ...);
void Std_FuncExit(const char *const, ...);
void Std_FuncEnter();

typedef struct Std_PerFileData
{
    int32_t initialized;
    const char *filename_psz;
    StdLevel current_std_level;
    StdLevel file_std_level;
} Std_PerFileData;
typedef Std_PerFileData *Std_PerFileDataPtr;

void Std_PerFileData_Init(Std_PerFileDataPtr const in_this, const char *const in_filename_psz, StdLevel const in_std_level);
void Std_PerFileData_Deinit(Std_PerFileDataPtr const in_this);

#ifdef _MSC_VER
#define FORCE_INLINE_DECLSPEC __forceinline
#define DEBUG_BREAK_FUNCTION DebugBreak
#endif
#ifdef __GNUC__
#define FORCE_INLINE_DECLSPEC __attribute__((always_inline))
#define DEBUG_BREAK_FUNCTION __builtin_trap
#endif

#ifndef STD_DISABLE
#define STD_FILE_SETUP_EX(IN_FILE_STD_DPF_LEVEL)                                       \
    static Std_PerFileData s_std_perFileData;                                          \
    static FORCE_INLINE_DECLSPEC StdLevel Std_PerFileData_GetCurrentLevel()            \
    {                                                                                  \
        if (s_std_perFileData.initialized == 0)                                        \
            Std_PerFileData_Init(&s_std_perFileData, __FILE__, IN_FILE_STD_DPF_LEVEL); \
        return s_std_perFileData.current_std_level;                                    \
    }
#define STD_FILE_SETUP() STD_FILE_SETUP_EX(StdLevel_Warning)

#define STD_CHECK_LEVEL(_level) (Std_PerFileData_GetCurrentLevel() >= _level)
#define STD_GOTO_FINALLY(_LEVEL)          \
    STD_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL); \
    goto finally;

#ifndef STD_ASSERT_DISABLE
#define STD_ASSERT(_EXPRESSION)                       \
    if (!(_EXPRESSION))                               \
    {                                                 \
        STD_SEVERE("%s", #_EXPRESSION);               \
        STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error); \
        DEBUG_BREAK_FUNCTION();                       \
    }
#else
#define STD_ASSERT(_EXPRESSION) __noop
#endif

#define STD_CHECK(_EXPRESSION)                        \
    if (!(_EXPRESSION))                               \
    {                                                 \
        STD_ERROR("%s", #_EXPRESSION);                \
        STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error); \
        goto finally;                                 \
    }
#define STD_CHECK_NO_GOTO(_EXPRESSION)                \
    if (!(_EXPRESSION))                               \
    {                                                 \
        STD_ERROR("%s", #_EXPRESSION);                \
        STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error); \
    }
#define STD_BOOL_TO_STRING(_BOOL) (_BOOL ? "TRUE" : "FALSE")

#define STD_SEVERE                        \
    if (STD_CHECK_LEVEL(StdLevel_Severe)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Severe), Std_Log
#define STD_ERROR                        \
    if (STD_CHECK_LEVEL(StdLevel_Error)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Error), Std_Log
#define STD_WARNING                        \
    if (STD_CHECK_LEVEL(StdLevel_Warning)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Warning), Std_Log
#define STD_NOTIFY                        \
    if (STD_CHECK_LEVEL(StdLevel_Notify)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Notify), Std_Log
#define STD_INFO                        \
    if (STD_CHECK_LEVEL(StdLevel_Info)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Info), Std_Log
#define STD_DEBUG                        \
    if (STD_CHECK_LEVEL(StdLevel_Debug)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Debug), Std_Log
#define STD_DEBUG_2                       \
    if (STD_CHECK_LEVEL(StdLevel_Debug2)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Debug2), Std_Log
#define STD_TRACE                        \
    if (STD_CHECK_LEVEL(StdLevel_Trace)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Trace), Std_Log
#define STD_TRACE_2                       \
    if (STD_CHECK_LEVEL(StdLevel_Trace2)) \
    Std_LogInit(__LINE__, __FUNCTION__, StdLevel_Trace2), Std_Log

#ifndef STD_DPF_ENTER_EXIT_FUNC_DISABLE
#define STD_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL) s_CurrentLevelFuncEnterExit = _LEVEL
#define STD_FUNC_ENTER                                      \
    StdLevel s_CurrentLevelFuncEnterExit = StdLevel_Trace2; \
    if (STD_CHECK_LEVEL(s_CurrentLevelFuncEnterExit))       \
    Std_LogInit(__LINE__, __FUNCTION__, s_CurrentLevelFuncEnterExit), Std_FuncEnter
#define STD_FUNC_EXIT                                 \
    if (STD_CHECK_LEVEL(s_CurrentLevelFuncEnterExit)) \
    Std_LogInit(__LINE__, __FUNCTION__, s_CurrentLevelFuncEnterExit), Std_FuncExit
#else
#define STD_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL)
#define STD_FUNC_ENTER __noop
#define STD_FUNC_EXIT __noop
#endif

#else
#define STD_FILE_SETUP_EX(_IN)
#define STD_FILE_SETUP()
#define STD_CHECK_LEVEL(_level)
#define STD_GOTO_FINALLY(_LEVEL) goto finally;
#define STD_ASSERT(_EXPRESSION)
#define STD_CHECK(_EXPRESSION) \
    if (!(_EXPRESSION))        \
        goto finally;
#define STD_CHECK_NO_GOTO(_EXPRESSION) (_EXPRESSION)
#define STD_BOOL_TO_STRING(_BOOL) (_BOOL ? "TRUE" : "FALSE")

#define STD_SEVERE __noop
#define STD_ERROR __noop
#define STD_WARNING __noop
#define STD_NOTIFY __noop
#define STD_INFO __noop
#define STD_DEBUG __noop
#define STD_DEBUG_2 __noop
#define STD_TRACE __noop
#define STD_TRACE_2 __noop

#define STD_SET_THIS_LEVEL_FUNC_EXIT(_LEVEL)
#define STD_FUNC_ENTER __noop
#define STD_FUNC_EXIT __noop

#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    void operator=(const TypeName &)

#ifdef __cplusplus
namespace TFG
{

namespace Foo
{
class Foo;
}
namespace Bar
{
class Bar;
}

class TFG
{
public:
    TFG(uint32_t const in_Param0);
    ~TFG();

private:
    DISALLOW_COPY_AND_ASSIGN(TFG);

    static uint32_t s_Param0;

    friend class Foo::Foo;
    friend class Bar::Bar;
};
#endif

#define call_CloseHandle ::CloseHandle
#define call_CreateEventA ::CreateEventA

} // namespace TFG
