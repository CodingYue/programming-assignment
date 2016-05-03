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

make_helper(push_ebp) {
	cpu.esp = cpu.esp - 0x4;
	swaddr_write(cpu.esp, 4, cpu.ebp);
	return 1;
}

make_helper(pop_ebp) {
	cpu.ebp = swaddr_read(cpu.esp, 4);
	cpu.esp += 0x4;
	return 1;
}