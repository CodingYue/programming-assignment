#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(pop_ebp) {
	cpu.ebp = swaddr_read(cpu.esp, 4);
	cpu.esp += 0x4;
	return 1;
}