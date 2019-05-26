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
#endif

#define PRIxPTR_WIDTH ((int32_t)(sizeof(uintptr_t) * 2))

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    void operator=(const TypeName &)

namespace TFG
{

class TFG
{
public:
    TFG(uint32_t const in_Param0);
    ~TFG();

private:
    DISALLOW_COPY_AND_ASSIGN(TFG);
};

} // namespace TFG
