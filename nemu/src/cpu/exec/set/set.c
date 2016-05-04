#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(set_ne) {
	uint32_t idx = instr_fetch(eip + 1, 1) >> 3 & 0x7;
	
	reg_b(idx) = ((cpu.EFLAGS & ZFLAG) == 0);
	return 2;
}