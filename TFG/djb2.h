#pragma once

#include "TFG_fwd.h"
#include "Sys.h"

#define TFG_DJB2_STEP(_HASH, _C) ((_HASH * 33) ^ _C)
//#define TFG_DJB2_STEP(_HASH, _C) (((_HASH << 5) + _HASH) ^ _C)
//#define TFG_DJB2_STEP(_HASH, _C) (((_HASH << 5) + _HASH) + _C)

#define TFG_DJB2_0 5381
#define TFG_DJB2_1(_C0) TFG_DJB2_STEP(TFG_DJB2_0, _C0)
#define TFG_DJB2_2(_C0, _C1) TFG_DJB2_STEP(TFG_DJB2_1(_C0), _C1)
#define TFG_DJB2_3(_C0, _C1, _C2) TFG_DJB2_STEP(TFG_DJB2_2(_C0, _C1), _C2)
#define TFG_DJB2_4(_C0, _C1, _C2, _C3) TFG_DJB2_STEP(TFG_DJB2_3(_C0, _C1, _C2), _C3)
#define TFG_DJB2_5(_C0, _C1, _C2, _C3, _C4) TFG_DJB2_STEP(TFG_DJB2_4(_C0, _C1, _C2, _C3), _C4)
#define TFG_DJB2_6(_C0, _C1, _C2, _C3, _C4, _C5) TFG_DJB2_STEP(TFG_DJB2_5(_C0, _C1, _C2, _C3, _C4), _C5)
#define TFG_DJB2_7(_C0, _C1, _C2, _C3, _C4, _C5, _C6) TFG_DJB2_STEP(TFG_DJB2_6(_C0, _C1, _C2, _C3, _C4, _C5), _C6)
#define TFG_DJB2_8(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7) TFG_DJB2_STEP(TFG_DJB2_7(_C0, _C1, _C2, _C3, _C4, _C5, _C6), _C7)
#define TFG_DJB2_9(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8) TFG_DJB2_STEP(TFG_DJB2_8(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7), _C8)
#define TFG_DJB2_10(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9) TFG_DJB2_STEP(TFG_DJB2_9(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8), _C9)
#define TFG_DJB2_11(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10) TFG_DJB2_STEP(TFG_DJB2_10(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9), _C10)
#define TFG_DJB2_12(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11) TFG_DJB2_STEP(TFG_DJB2_11(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10), _C11)
#define TFG_DJB2_13(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12) TFG_DJB2_STEP(TFG_DJB2_12(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11), _C12)
#define TFG_DJB2_14(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13) TFG_DJB2_STEP(TFG_DJB2_13(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12), _C13)
#define TFG_DJB2_15(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14) TFG_DJB2_STEP(TFG_DJB2_14(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13), _C14)
#define TFG_DJB2_16(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15) TFG_DJB2_STEP(TFG_DJB2_15(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14), _C15)
#define TFG_DJB2_17(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16) TFG_DJB2_STEP(TFG_DJB2_16(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15), _C16)
#define TFG_DJB2_18(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17) TFG_DJB2_STEP(TFG_DJB2_17(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16), _C17)
#define TFG_DJB2_19(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17, _C18) TFG_DJB2_STEP(TFG_DJB2_18(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17), _C18)
#define TFG_DJB2_20(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17, _C18, _C19) TFG_DJB2_STEP(TFG_DJB2_19(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17, _C18), _C19)

inline uint32_t djb2(unsigned char const *str)
{
	uint32_t hash = TFG_DJB2_0;
	int32_t c;

	while ((c = *str++))
		hash = TFG_DJB2_STEP(hash, c);

	return hash;
}
