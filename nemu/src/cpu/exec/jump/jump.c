#include "cpu/exec/helper.h"

#define jmp_helper(CONDITION, CNAME, CONDITION_FLAG) \
	make_helper(concat4(j, CONDITION, _, SUFFIX)) { \
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE); \
		if (CONDITION_FLAG) cpu.eip += rel; \
		print_asm("j%s 0x%x", CNAME, cpu.eip+DATA_BYTE+1); \
		return DATA_BYTE+1; \
	}


#define ALL_JMP \
	jmp_helper(mp, "mp", (true)) \
	jmp_helper(a, "a", (CFLAG == 0 && ZFLAG == 0)) \
	jmp_helper(ae, "ae", (CFLAG == 0)) \
	jmp_helper(b, "b", (CFLAG == 1)) \
	jmp_helper(be, "be", (CFLAG == 1 || ZFLAG == 1)) \
	jmp_helper(e, "e", (ZFLAG == 1)) \
	jmp_helper(ne, "ne", (ZFLAG == 0)) \
	jmp_helper(l, "l", (SFLAG != OFLAG)) \
	jmp_helper(le, "le", (ZFLAG == 1 || SFLAG != OFLAG)) \
	jmp_helper(g, "g", (ZFLAG == 0 && SFLAG == OFLAG)) \
	jmp_helper(ge, "ge", (SFLAG == OFLAG))

#define DATA_BYTE 1

#include "cpu/exec/template-start.h"
ALL_JMP
#include "cpu/exec/template-end.h"

#undef DATA_BYTE

#define DATA_BYTE 2

#include "cpu/exec/template-start.h"
ALL_JMP
#include "cpu/exec/template-end.h"

#undef DATA_BYTE

#define DATA_BYTE 4

#include "cpu/exec/template-start.h"
ALL_JMP
#include "cpu/exec/template-end.h"

#undef DATA_BYTE


make_helper_v(jmp)
make_helper_v(ja)
make_helper_v(jae)
make_helper_v(jb)
make_helper_v(jbe)
make_helper_v(je)
make_helper_v(jne)
make_helper_v(jl)
make_helper_v(jle)
make_helper_v(jg)
make_helper_v(jge)


make_helper(call) {
	int32_t rel;
	int len = ops_decoded.is_data_size_16 ? 2 : 4;
	rel = ops_decoded.is_data_size_16 ? (int16_t) instr_fetch(eip + 1, len) 
		  : (int32_t) instr_fetch(eip + 1, len);
	
	cpu.esp = cpu.esp - 0x4;
	swaddr_write(cpu.esp, 4, cpu.eip+len+1);
	cpu.eip += rel;

	print_asm("call 0x%x", cpu.eip+len+1);

	return len+1;
}
