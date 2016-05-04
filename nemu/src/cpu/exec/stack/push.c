#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(push_ebp) {
	cpu.esp = cpu.esp - 0x4;
	swaddr_write(cpu.esp, 4, cpu.ebp);
	return 1;
}

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE


make_helper_v(push_rm);
make_helper_v(push_i);