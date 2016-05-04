#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	DATA_TYPE result = ~op_src->val;
	OPERAND_W(op_src, result);
	print_asm_template1();
}

make_instr_helper(rm);
make_instr_helper(r);
make_instr_helper(i);

#include "cpu/exec/template-end.h"
