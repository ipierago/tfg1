#pragma once
#ifdef WIN32

#include <Call/call_fwd.h>
#include <Sys/sys.h>

CALL_API HANDLE call_CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
CALL_API BOOL call_CloseHandle(HANDLE hObject);
CALL_API HANDLE call_CreateThread(LPSECURITY_ATTRIBUTES  lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
CALL_API BOOL call_TerminateThread(HANDLE hThread, DWORD dwExitCode);
CALL_API BOOL call_SetEvent(HANDLE hEvent);
CALL_API DWORD call_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
CALL_API DWORD call_WaitForMultipleObjects(DWORD nCount, CONST HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);
CALL_API void call_InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
CALL_API void call_EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
CALL_API void call_LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
CALL_API void call_DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
CALL_API HANDLE call_CreateIoCompletionPort(HANDLE FileHandle, HANDLE ExistingCompletionPort, ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads);
CALL_API BOOL call_GetQueuedCompletionStatus(HANDLE CompletionPort,LPDWORD lpNumberOfBytesTransferred, PULONG_PTR lpCompletionKey, LPOVERLAPPED * lpOverlapped, DWORD dwMilliseconds);
CALL_API BOOL call_PostQueuedCompletionStatus(HANDLE CompletionPort, DWORD dwNumberOfBytesTransferred, ULONG_PTR dwCompletionKey, LPOVERLAPPED lpOverlapped );

CALL_API VOID call_InitializeThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe);
CALL_API VOID call_DestroyThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe);
CALL_API PTP_IO call_CreateThreadpoolIo(HANDLE fl, PTP_WIN32_IO_CALLBACK pfnio, PVOID pv, PTP_CALLBACK_ENVIRON pcbe);
CALL_API VOID call_CloseThreadpoolIo(PTP_IO pio);
CALL_API VOID call_StartThreadpoolIo(PTP_IO pio);
CALL_API VOID call_CancelThreadpoolIo(PTP_IO pio);
CALL_API VOID call_WaitForThreadpoolIoCallbacks(PTP_IO pio, BOOL fCancelPendingCallbacks);
CALL_API PTP_WORK call_CreateThreadpoolWork(PTP_WORK_CALLBACK pfnwk, PVOID pv, PTP_CALLBACK_ENVIRON pcbe);
CALL_API VOID call_WaitForThreadpoolWorkCallbacks(PTP_WORK pwk,BOOL fCancelPendingCallbacks);
CALL_API VOID call_CloseThreadpoolWork(PTP_WORK pwk);
CALL_API VOID call_SubmitThreadpoolWork(PTP_WORK pwk);
CALL_API PTP_POOL call_CreateThreadpool(PVOID reserved);
CALL_API PTP_TIMER call_CreateThreadpoolTimer(PTP_TIMER_CALLBACK pfnti, PVOID pv, PTP_CALLBACK_ENVIRON pcbe );
CALL_API BOOL call_SetConsoleCtrlHandler(PHANDLER_ROUTINE HandlerRoutine, BOOL Add);
#endif
