#pragma once

#include "fwd.h"

VOID CALLBACK net_tcpconnector_io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io);

extern Std_TypeId net_tcpconnector_typeid;

void net_tcpconnector_on_queued_completion_status(NetNT_TCPConnectorPtr const in_this, BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p);
