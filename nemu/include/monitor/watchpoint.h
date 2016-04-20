#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char *expr;
	int value;
} WP;

void new_wp(char *, int);
int free_wp(int);
void show_watchpoints();

#endif
