#pragma once

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#ifdef WIN32
#include <conio.h>
#endif
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
#include <algorithm>
#include <atomic>
#include <mutex>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#endif

#define PRIxPTR_WIDTH ((int32_t)(sizeof(uintptr_t) * 2))

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    void operator=(const TypeName &)

typedef uint32_t Std_TypeId;

#define STD_STRUCT_PTR_FROM_MEMBER_PTR(IN_MEMBER_PTR, IN_STRUCT_TYPE, IN_MEMBER_NAME) (IN_STRUCT_TYPE *)(((char *)IN_MEMBER_PTR) - offsetof(IN_STRUCT_TYPE, IN_MEMBER_NAME));

#ifdef _MSC_VER
#define FORCE_INLINE_DECLSPEC __forceinline
#define DEBUG_BREAK_FUNCTION DebugBreak
#endif
#ifdef __GNUC__
#define FORCE_INLINE_DECLSPEC __attribute__((always_inline))
#define DEBUG_BREAK_FUNCTION __builtin_trap
#endif

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

#define call_CloseHandle ::CloseHandle
#define call_CreateEventA ::CreateEventA
#define StdResult HRESULT

} // namespace TFG
