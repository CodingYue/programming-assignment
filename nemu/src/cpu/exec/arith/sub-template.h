#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest, result);

	panic("please implement me");
	print_asm_template1();

}


#include "cpu/exec/template-end.h"