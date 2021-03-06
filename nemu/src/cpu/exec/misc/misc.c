#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(leave) {
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp, 4);
	cpu.esp += 8;

	print_asm("leave");

	return 1;
}

make_helper(ret) {

	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	print_asm("ret");

	return 0;
}

make_helper(ret_w) {
	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 0x4;
	int16_t rel16 = (int16_t) instr_fetch(eip + 1, 2);
	cpu.esp += rel16;

	print_asm("ret %d", rel16);

	return 0;
}

make_helper(cltd) {
	cpu.edx = (cpu.eax >> 31 & 1) ? 0xFFFFFFFF : 0;
	return 1;
}