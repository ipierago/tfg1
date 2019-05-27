#pragma once

#include "TFG_fwd.h"
#include "Sys.h"

#define STD_DJB2_STEP(_HASH, _C) ((_HASH * 33) ^ _C)
//#define STD_DJB2_STEP(_HASH, _C) (((_HASH << 5) + _HASH) ^ _C)
//#define STD_DJB2_STEP(_HASH, _C) (((_HASH << 5) + _HASH) + _C)

#define STD_DJB2_0 5381
#define STD_DJB2_1(_C0) STD_DJB2_STEP(STD_DJB2_0, _C0)
#define STD_DJB2_2(_C0, _C1) STD_DJB2_STEP(STD_DJB2_1(_C0), _C1)
#define STD_DJB2_3(_C0, _C1, _C2) STD_DJB2_STEP(STD_DJB2_2(_C0, _C1), _C2)
#define STD_DJB2_4(_C0, _C1, _C2, _C3) STD_DJB2_STEP(STD_DJB2_3(_C0, _C1, _C2), _C3)
#define STD_DJB2_5(_C0, _C1, _C2, _C3, _C4) STD_DJB2_STEP(STD_DJB2_4(_C0, _C1, _C2, _C3), _C4)
#define STD_DJB2_6(_C0, _C1, _C2, _C3, _C4, _C5) STD_DJB2_STEP(STD_DJB2_5(_C0, _C1, _C2, _C3, _C4), _C5)
#define STD_DJB2_7(_C0, _C1, _C2, _C3, _C4, _C5, _C6) STD_DJB2_STEP(STD_DJB2_6(_C0, _C1, _C2, _C3, _C4, _C5), _C6)
#define STD_DJB2_8(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7) STD_DJB2_STEP(STD_DJB2_7(_C0, _C1, _C2, _C3, _C4, _C5, _C6), _C7)
#define STD_DJB2_9(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8) STD_DJB2_STEP(STD_DJB2_8(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7), _C8)
#define STD_DJB2_10(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9) STD_DJB2_STEP(STD_DJB2_9(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8), _C9)
#define STD_DJB2_11(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10) STD_DJB2_STEP(STD_DJB2_10(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9), _C10)
#define STD_DJB2_12(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11) STD_DJB2_STEP(STD_DJB2_11(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10), _C11)
#define STD_DJB2_13(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12) STD_DJB2_STEP(STD_DJB2_12(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11), _C12)
#define STD_DJB2_14(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13) STD_DJB2_STEP(STD_DJB2_13(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12), _C13)
#define STD_DJB2_15(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14) STD_DJB2_STEP(STD_DJB2_14(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13), _C14)
#define STD_DJB2_16(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15) STD_DJB2_STEP(STD_DJB2_15(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14), _C15)
#define STD_DJB2_17(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16) STD_DJB2_STEP(STD_DJB2_16(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15), _C16)
#define STD_DJB2_18(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17) STD_DJB2_STEP(STD_DJB2_17(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16), _C17)
#define STD_DJB2_19(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17, _C18) STD_DJB2_STEP(STD_DJB2_18(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17), _C18)
#define STD_DJB2_20(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17, _C18, _C19) STD_DJB2_STEP(STD_DJB2_19(_C0, _C1, _C2, _C3, _C4, _C5, _C6, _C7, _C8, _C9, _C10, _C11, _C12, _C13, _C14, _C15, _C16, _C17, _C18), _C19)

inline uint32_t djb2(unsigned char const *str)
{
	uint32_t hash = STD_DJB2_0;
	int32_t c;

	while ((c = *str++))
		hash = STD_DJB2_STEP(hash, c);

	return hash;
}
