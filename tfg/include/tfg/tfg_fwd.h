#pragma once

#include "sys.h"

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (P)
#endif

#ifdef _MSC_VER
#define THREAD_LOCAL_STORAGE_DECLSPEC __declspec(thread)
#endif

#ifdef __GNUC__
#define THREAD_LOCAL_STORAGE_DECLSPEC __thread
#endif

#define M_CONC(A, B) M_CONC_(A, B)
#define M_CONC_(A, B) A##B
#define M_TYPEDEF_STRUCT(_IN_IDENTIFIER) typedef struct _IN_IDENTIFIER _IN_IDENTIFIER

#define D_Ptr Ptr
#define D_CPtr CPtr
#define D_PtrPtr PtrPtr
#define D__ _
#define D_
#define D__API _API
#define D_Vtbl Vtbl
#define D_VtblCPtr VtblCPtr
#define D_I I

#ifndef _WIN32
typedef void *HANDLE;
typedef long LONG_PTR, *PLONG_PTR;
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#endif

typedef long TFG_Result;

#ifndef WIN32
#define S_OK ((TFG_Result)0L)
#define E_FAIL ((TFG_Result)(0x80004005L))
#define SUCCEEDED(hr) (((TFG_Result)(hr)) >= 0)
#define FAILED(hr) (((TFG_Result)(hr)) < 0)
#endif

typedef uint32_t TFG_TypeId;

#define TFG_STRUCT_PTR_FROM_MEMBER_PTR(IN_MEMBER_PTR, IN_STRUCT_TYPE, IN_MEMBER_NAME) (IN_STRUCT_TYPE *)(((char *)IN_MEMBER_PTR) - offsetof(IN_STRUCT_TYPE, IN_MEMBER_NAME));
