#pragma once

#include "Sys.h"

#ifdef __cplusplus
#define STD_EXTERN_C extern "C"
#else
#define STD_EXTERN_C
#endif

#define STD_DECLSPEC
#ifdef std_EXPORTS
#ifdef WIN32
#undef STD_DECLSPEC
#define STD_DECLSPEC __declspec(dllexport)
#endif
#endif

#define STD_LINKAGE STD_EXTERN_C STD_DECLSPEC
#define STD_API STD_LINKAGE

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

#define STD_STRUCT_TYPEDEF_EX(_IN_LINKAGE, _IN_IDENTIFIER)        \
	_IN_LINKAGE typedef struct _IN_IDENTIFIER _IN_IDENTIFIER;     \
	typedef _IN_IDENTIFIER *M_CONC(_IN_IDENTIFIER, D_Ptr);        \
	typedef _IN_IDENTIFIER const *M_CONC(_IN_IDENTIFIER, D_CPtr); \
	typedef M_CONC(_IN_IDENTIFIER, D_Ptr) * M_CONC(_IN_IDENTIFIER, D_PtrPtr);

#ifdef __cplusplus
#define STD_MODULE_OBJECT_FWD_1(_IN_LINKAGE, _IN_MODULE_IDENTIFIER, _IN_IDENTIFIER) \
	namespace _IN_MODULE_IDENTIFIER                                                 \
	{                                                                               \
	_IN_LINKAGE typedef struct _IN_IDENTIFIER _IN_IDENTIFIER;                       \
	_IN_LINKAGE typedef _IN_IDENTIFIER *M_CONC(_IN_IDENTIFIER, D_Ptr);              \
	_IN_LINKAGE typedef _IN_IDENTIFIER const *M_CONC(_IN_IDENTIFIER, D_CPtr);       \
	}
#else
#define STD_MODULE_OBJECT_FWD_1(_IN_LINKAGE, _IN_MODULE_IDENTIFIER, _IN_IDENTIFIER)
#endif

#define STD_MODULE_OBJECT_FWD_2(_IN_LINKAGE, _IN_MODULE_IDENTIFIER, _IN_IDENTIFIER)                                                                             \
	_IN_LINKAGE typedef struct M_CONC(M_CONC(_IN_MODULE_IDENTIFIER, D__), _IN_IDENTIFIER) M_CONC(M_CONC(_IN_MODULE_IDENTIFIER, D__), _IN_IDENTIFIER);           \
	_IN_LINKAGE typedef M_CONC(M_CONC(_IN_MODULE_IDENTIFIER, D__), _IN_IDENTIFIER) * M_CONC(M_CONC(M_CONC(_IN_MODULE_IDENTIFIER, D__), _IN_IDENTIFIER), D_Ptr); \
	_IN_LINKAGE typedef M_CONC(M_CONC(_IN_MODULE_IDENTIFIER, D__), _IN_IDENTIFIER) const *M_CONC(M_CONC(M_CONC(_IN_MODULE_IDENTIFIER, D__), _IN_IDENTIFIER), D_CPtr);

#define STD_MODULE_OBJECT_FWD(_IN_LINKAGE, _IN_MODULE_IDENTIFIER, _IN_IDENTIFIER) \
	STD_MODULE_OBJECT_FWD_1(_IN_LINKAGE, _IN_MODULE_IDENTIFIER, _IN_IDENTIFIER)   \
	STD_MODULE_OBJECT_FWD_2(_IN_LINKAGE, _IN_MODULE_IDENTIFIER, _IN_IDENTIFIER)

#define STD_MODULE_INTERFACE_FWD(_IN_LINKAGE, _IN_MODULE, _IN_INTERFACE)                               \
	STD_STRUCT_TYPEDEF_EX(_IN_LINKAGE, M_CONC(M_CONC(M_CONC(_IN_MODULE, D__), _IN_INTERFACE), D_Vtbl)) \
	_IN_LINKAGE typedef M_CONC(M_CONC(M_CONC(_IN_MODULE, D__), _IN_INTERFACE), D_VtblCPtr) const *M_CONC(M_CONC(M_CONC(_IN_MODULE, D__), _IN_INTERFACE), D_I);

// STD_STRUCT_TYPEDEFS is deprecated
#define STD_STRUCT_TYPEDEFS(_IN_IDENTIFIER)                \
	typedef struct _IN_IDENTIFIER _IN_IDENTIFIER;          \
	typedef _IN_IDENTIFIER *M_CONC(_IN_IDENTIFIER, D_Ptr); \
	typedef _IN_IDENTIFIER const *M_CONC(_IN_IDENTIFIER, D_CPtr);

typedef long StdResult;
#ifndef WIN32
#define S_OK ((StdResult)0L)
#define E_FAIL ((StdResult)(0x80004005L))
#define SUCCEEDED(hr) (((StdResult)(hr)) >= 0)
#define FAILED(hr) (((StdResult)(hr)) < 0)
#endif

#define STD1_MODULE_FWD(_IN_LINKAGE, _IN_MODULE)

#define STD1_MODULE_OBJECT_FWD(_IN_LINKAGE, _IN_MODULE, _IN_OBJECT)                                                                       \
	_IN_LINKAGE typedef struct M_CONC(M_CONC(_IN_MODULE, D__), _IN_IDENTIFIER) M_CONC(M_CONC(_IN_MODULE, D__), _IN_IDENTIFIER);           \
	_IN_LINKAGE typedef M_CONC(M_CONC(_IN_MODULE, D__), _IN_IDENTIFIER) * M_CONC(M_CONC(M_CONC(_IN_MODULE, D__), _IN_IDENTIFIER), D_Ptr); \
	_IN_LINKAGE typedef M_CONC(M_CONC(_IN_MODULE, D__), _IN_IDENTIFIER) const *M_CONC(M_CONC(M_CONC(_IN_MODULE, D__), _IN_IDENTIFIER), D_CPtr);

//STD_STRUCT_TYPEDEF_EX(_IN_LINKAGE, M_CONC(M_CONC(_IN_MODULE, D__), _IN_IDENTIFIER))

typedef uint32_t Std_TypeId;

#define STD_STRUCT_PTR_FROM_MEMBER_PTR(IN_MEMBER_PTR, IN_STRUCT_TYPE, IN_MEMBER_NAME) (IN_STRUCT_TYPE *)(((char *)IN_MEMBER_PTR) - offsetof(IN_STRUCT_TYPE, IN_MEMBER_NAME));
