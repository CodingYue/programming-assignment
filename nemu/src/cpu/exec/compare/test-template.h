#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {

	update_EFLAGS(op_dest->val);
	print_asm_template1();

}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"