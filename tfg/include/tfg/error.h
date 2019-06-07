#pragma once

#include "tfg_fwd.h"
#include "sys.h"
const char *TFG_WSAErrorToStringShort(int32_t const rv);
const char *TFG_ResultToStringShort(TFG_Result const);
char const *TFG_GetErrorString(uint32_t const in_error_code);

#define TFG_ResultToStringShort TFG_ResultToStringShort
