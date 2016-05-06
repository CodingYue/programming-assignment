#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

int get_reg(const char *name) {
	int R;
	for (R = 0; R < 8; ++R) {
		if (strcmp(name, regsl[R]) == 0) return reg_l(R);
		if (strcmp(name, regsw[R]) == 0) return reg_w(R);
		if (strcmp(name, regsb[R]) == 0) return reg_b(R);
	}
	if (strcmp(name, "eip") == 0) return cpu.eip;
	Log("register name not matched");
	return 0;
}


void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

void update_EFLAGS(uint64_t result, int DATA_BYTE) {
	cpu.EFLAGS = 0x2;

	int len = (8 * DATA_BYTE);
	cpu.EFLAGS |= (result >> len & 1) << 0;
	cpu.EFLAGS |= (result & 1) << 2;
	cpu.EFLAGS |= (!result) << 6;
	cpu.EFLAGS |= (result >> (len-1) & 1) << 7;
	cpu.EFLAGS |= ((result >> (len-1) & 1) ^ (result >> len & 1)) << 11;
}
