#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	uint32_t dest = op_dest->val;
	uint32_t src = op_src->val;
	uint64_t result = (uint64_t) src + dest;
	OPERAND_W(op_dest, (uint32_t) result);

	update_EFLAGS(result);
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