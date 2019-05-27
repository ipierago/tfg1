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

#ifdef ANDROID
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <android/log.h>
#endif

#ifdef _WIN64
    //define something for Windows (64-bit)
#elif _WIN32
    //define something for Windows (32-bit)
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
        // define something for simulator
    #elif TARGET_OS_IPHONE
        // define something for iphone
        #include <OpenGLES/ES3/gl.h>
    #else
        #define TARGET_OS_OSX 1
        // define something for OSX
    #endif
#elif __ANDROID__
    #include <GLES3/gl3.h>
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
#elif __linux
    // linux
#elif __unix // all unices not caught above
    // Unix
#elif __posix
    // POSIX
#endif

// TODO: Compiler/platform specific stuff here
#ifndef _WIN64
#ifndef _WIN32
    typedef int errno_t;
    typedef size_t rsize_t;
    typedef struct _WSABUF {
        uint32_t len;     /* the length of the buffer */
        char * buf; /* the pointer to the buffer */
    } WSABUF, * LPWSABUF;
    typedef void *LPVOID;
#endif
#endif
