#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	swaddr_write(cpu.esp, 4, op_src->val);
	cpu.esp += 0x4;
	print_asm_template1();
}

make_instr_helper(rm);
make_instr_helper(r);
make_instr_helper(i);

#include "cpu/exec/template-end.h"
