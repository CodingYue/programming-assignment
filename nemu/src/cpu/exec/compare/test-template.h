#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {

	update_EFLAGS(op_dest->val & op_src->val, DATA_BYTE);
	print_asm_template1();

}

make_instr_helper(r2rm);
make_instr_helper(i2rm);
make_instr_helper(i2a);

#include "cpu/exec/template-end.h"	