#pragma once

#include "TFG_fwd.h"
#include "Sys.h"
const char *Std_WSAErrorToStringShort(int32_t const rv);
const char *Std_ResultToStringShort(StdResult const);
char const *Std_GetErrorString(uint32_t const in_error_code);

#define Std_ResultToStringShort Std_ResultToStringShort
