#include "cpu/exec/helper.h"

make_helper(call) {
	uint32_t rel;
	if (ops_decoded.is_data_size_16) {
		rel = instr_fetch(eip + 1, 2);
		cpu.eip += 3;
	} else {
		rel = instr_fetch(eip + 1, 4);
		cpu.eip += 5;
	}

	cpu.esp = cpu.esp - 0x4;
	swaddr_write(cpu.esp, 4, cpu.eip);
	cpu.eip += rel;
	return 0;
}

make_helper(je) {
	uint32_t rel8 = instr_fetch(eip + 1, 1);
	if (cpu.EFLAGS & ZFLAG) cpu.eip += rel8;
	return 2;
}