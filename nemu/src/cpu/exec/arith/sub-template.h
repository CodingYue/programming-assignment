#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE dest = op_dest->val;
	DATA_TYPE src = ~op_src->val + 1;
	uint64_t result = (uint64_t) src + dest;
	
	OPERAND_W(op_dest, (DATA_TYPE) result);

	update_EFLAGS(result, DATA_BYTE);
	print_asm_template2();

}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"