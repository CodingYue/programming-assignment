#include "cpu/exec/template-start.h"
#include "cpu/exec/helper.h"

#define jmp_helper(CONDITION, CNAME, CONDITION_FLAG) \
	make_helper(concat4(j, CONDITION, _, SUFFIX)) { \
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE); \
		if (CONDITION_FLAG) cpu.eip += rel; \
		print_asm("j%s 0x%x", CNAME, cpu.eip+DATA_BYTE+1); \
		return DATA_BYTE+1; \
	}
	

#include "cpu/exec/template-end.h"