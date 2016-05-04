#include "cpu/exec/helper.h"

make_helper(call) {
	uint32_t rel;
	int len = ops_decoded.is_data_size_16 ? 2 : 4;
	rel = instr_fetch(eip + 1, len);
	
	cpu.esp = cpu.esp - 0x4;
	swaddr_write(cpu.esp, 4, cpu.eip+len+1);
	cpu.eip += rel;
	return len+1;
}

make_helper(jmp_b) {
	uint32_t rel8 = instr_fetch(eip + 1, 1);
	cpu.eip += rel8;
	return 2;
}

make_helper(jmp_v) {
	uint32_t rel;
	int len = ops_decoded.is_data_size_16 ? 2 : 4;
	rel = instr_fetch(eip + 1, len);
	cpu.eip += rel;
	return len+1;
}


/* condition jmp */

make_helper(je) {
	uint32_t rel8 = instr_fetch(eip + 1, 1);
	if (ZFLAG) cpu.eip += rel8;
	return 2;
}

make_helper(jbe) {
	uint32_t rel8 = instr_fetch(eip + 1, 1);
	if (ZFLAG || CFLAG) cpu.eip += rel8;
	return 2;
}

make_helper(jne) {
	uint32_t rel8 = instr_fetch(eip + 1, 1);
	if (!ZFLAG) cpu.eip += rel8;
	return 2;	
}

make_helper(jle) {
	uint32_t rel8 = instr_fetch(eip + 1, 1);
	if (ZFLAG || SFLAG != OFLAG) cpu.eip += rel8;
	return 2;
}