#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define DATA_BYTE 1
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE


make_helper_v(pop_rm);
make_helper_v(pop_i);
make_helper_v(pop_r);