#include "cpu/exec/helper.h"

#define cmovcc_helper(CONDITION, CNAME, CONDITION_FLAG) \
	make_helper(concat4(cmov, CONDITION, _, SUFFIX)) { \
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE); \
		if (CONDITION_FLAG) cpu.eip += rel; \
		print_asm("cmov%s 0x%x", CNAME, cpu.eip+DATA_BYTE+1); \
		return DATA_BYTE+1; \
	}


#define ALL_cmov \
	cmovcc_helper(mp, "mp", (true)) \
	cmovcc_helper(a, "a", (CFLAG == 0 && ZFLAG == 0)) \
	cmovcc_helper(ae, "ae", (CFLAG == 0)) \
	cmovcc_helper(b, "b", (CFLAG == 1)) \
	cmovcc_helper(be, "be", (CFLAG == 1 || ZFLAG == 1)) \
	cmovcc_helper(e, "e", (ZFLAG == 1)) \
	cmovcc_helper(ne, "ne", (ZFLAG == 0)) \
	cmovcc_helper(l, "l", (SFLAG != OFLAG)) \
	cmovcc_helper(le, "le", (ZFLAG == 1 || SFLAG != OFLAG)) \
	cmovcc_helper(g, "g", (ZFLAG == 0 && SFLAG == OFLAG)) \
	cmovcc_helper(ge, "ge", (SFLAG == OFLAG)) \

#define DATA_BYTE 1

#include "cpu/exec/template-start.h"
ALL_cmov
#include "cpu/exec/template-end.h"

#undef DATA_BYTE

#define DATA_BYTE 2

#include "cpu/exec/template-start.h"
ALL_cmov
#include "cpu/exec/template-end.h"

#undef DATA_BYTE

#define DATA_BYTE 4

#include "cpu/exec/template-start.h"
ALL_cmov
#include "cpu/exec/template-end.h"

#undef DATA_BYTE


make_helper_v(cmovmp);
make_helper_v(cmova);
make_helper_v(cmovae);
make_helper_v(cmovb);
make_helper_v(cmovbe);
make_helper_v(cmove);
make_helper_v(cmovne);
make_helper_v(cmovl);
make_helper_v(cmovle);
make_helper_v(cmovg);
make_helper_v(cmovge);