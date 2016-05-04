#include "cpu/exec/helper.h"



make_helper(call) {
	int32_t rel;
	int len = ops_decoded.is_data_size_16 ? 2 : 4;
	rel = ops_decoded.is_data_size_16 ? (int16_t) instr_fetch(eip + 1, len) 
		  : (int32_t) instr_fetch(eip + 1, len);
	
	cpu.esp = cpu.esp - 0x4;
	swaddr_write(cpu.esp, 4, cpu.eip+len+1);
	cpu.eip += rel;

	print_asm("call 0x%x", cpu.eip+len+1);

	return len+1;
}

make_helper(jmp_b) {
	int8_t rel8 = instr_fetch(eip + 1, 1);
	cpu.eip += rel8;

	print_asm("jmp 0x%x", cpu.eip+2);

	return 2;
}

make_helper(jmp_v) {
	int32_t rel;
	int len = ops_decoded.is_data_size_16 ? 2 : 4;
	rel = ops_decoded.is_data_size_16 ? (int16_t) instr_fetch(eip + 1, len) 
		  : (int32_t) instr_fetch(eip + 1, len);

	cpu.eip += rel;

	print_asm("jmp 0x%x", cpu.eip+len+1);

	return len+1;
}


/* condition jmp */

make_helper(je) {
	int8_t rel8 = instr_fetch(eip + 1, 1);
	if (ZFLAG) cpu.eip += rel8;

	print_asm("je 0x%x", cpu.eip+2);

	return 2;
}

make_helper(jbe) {
	int8_t rel8 = instr_fetch(eip + 1, 1);
	if (ZFLAG || CFLAG) cpu.eip += rel8;

	print_asm("jbe 0x%x", cpu.eip+2);

	return 2;
}

make_helper(jne) {
	int8_t rel8 = instr_fetch(eip + 1, 1);
	if (!ZFLAG) cpu.eip += rel8;

	print_asm("jne 0x%x", cpu.eip+2);

	return 2;	
}

make_helper(jle) {
	int8_t rel8 = instr_fetch(eip + 1, 1);
	if (ZFLAG || SFLAG != OFLAG) cpu.eip += rel8;

	print_asm("jle 0x%x", cpu.eip+2);

	return 2;
}

make_helper(jg) {
	int8_t rel8 = instr_fetch(eip + 1, 1);
	if (ZFLAG == 0 && SFLAG == OFLAG) cpu.eip += rel8;

	print_asm("jg 0x%x", cpu.eip+2);

	return 2;
}