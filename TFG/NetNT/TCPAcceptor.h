#pragma once

#include "fwd.h"

extern Std_TypeId net_tcpacceptor_typeid;

void net_tcpacceptor_on_queued_completion_status(NetNT_TCPAcceptorPtr const in_this, BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p);
