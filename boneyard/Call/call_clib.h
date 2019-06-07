#pragma once

#include <Call/call_fwd.h>
#include <Sys/sys.h>

CALL_API void * call_malloc(int32_t const in_Size);
CALL_API void call_free(void * const in_pv);
CALL_API void * call_memset(void *dest, int32_t c, size_t count);
CALL_API void* call_memcpy(void* _Dst, void const* _Src, size_t _Size);
CALL_API errno_t call_strcpy_s(char* _Destination, rsize_t _SizeInBytes, char const* _Source);

