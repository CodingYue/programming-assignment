#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper(movsxd_l) {

	int len = decode_rm2r_w(eip);
	reg_l(op_dest->reg) = (int16_t)op_src->val;

	return len;
}

make_helper(movzxd_l) {

	int len = decode_rm2r_w(eip);
	reg_l(op_dest->reg) = (uint16_t) op_src->val;

	return len;
}

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
make_helper_v(movs)
make_helper_v(movsx)
make_helper_v(movzx)