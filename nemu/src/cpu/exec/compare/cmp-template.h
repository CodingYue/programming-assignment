#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {

	uint64_t result = op_dest->val - op_src->val;
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