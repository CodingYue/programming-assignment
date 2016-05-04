#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI, R_EFLAGS};
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* DONE: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {

	union {
		struct {
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};

		struct {
			union {
				uint32_t _32;
				uint16_t _16;
				uint8_t _8[2];
			} gpr[8];
		};

	};
	swaddr_t eip;
	uint32_t EFLAGS;

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

int get_reg(const char *name);

#define CFLAG (cpu.EFLAGS >> 0 & 1)
#define PFLAG (cpu.EFLAGS >> 2 & 1)
#define AFLAG (cpu.EFLAGS >> 4 & 1)
#define ZFLAG (cpu.EFLAGS >> 6 & 1)
#define SFLAG (cpu.EFLAGS >> 7 & 1)
#define TFLAG (cpu.EFLAGS >> 8 & 1)
#define IFLAG (cpu.EFLAGS >> 9 & 1)
#define DFLAG (cpu.EFLAGS >> 10 & 1)
#define OFLAG (cpu.EFLAGS >> 11 & 1)

#endif
