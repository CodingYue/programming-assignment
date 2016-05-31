#ifndef __NEMU_H__
#define __NEMU_H__

#include "common.h"
#include "memory/memory.h"
#include "cpu/reg.h"

int variable_value(const char *, bool *);
char *func_name(swaddr_t);
#endif
