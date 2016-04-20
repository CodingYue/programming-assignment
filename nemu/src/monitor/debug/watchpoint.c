#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <string.h>
#include <stdlib.h>

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
		wp_list[i].expr = NULL;
	}
	wp_list[NR_WP - 1].next = NULL;

	free_ = wp_list;
}

void new_wp(char *e, int value) {
	Assert(free_ != NULL, "watchpoint pool overflow!");
	WP *wp = free_;
	
	int len = strlen(e);
	wp->expr = (char *) malloc((len+1) * sizeof(char));
	strcpy(wp->expr, e);

	wp->value = value;
	free_ = free_->next;
}

int free_wp(int NO) {
	WP *wp = &wp_list[NO];
	if (wp->expr == NULL) return -1;
	free(wp->expr);
	wp->expr = NULL;
	wp->next = free_;
	free_ = wp;
	return 0;
}

void show_watchpoints() {
	int i;
	for (i = 0; i < NR_WP; ++i) {
		if (wp_list[i].expr == NULL) continue;
		printf("No.%d, expr is (%s), expr value is %d\n", i, wp_list[i].expr, wp_list[i].value);
	}
}

int check_watchpoints() {
	int cnt = 0;
	int i;
	for (i = 0; i < NR_WP; ++i) {
		if (wp_list[i].expr == NULL) continue;
		bool success;
		int cur_value = expr(wp_list[i].expr, &success);
		if (cur_value == wp_list[i].value) continue;
		printf("No.%d, expr is (%s), expr previous value is %d, current value is %d\n", i, wp_list[i].expr, wp_list[i].value, cur_value);	
		wp_list[i].value = cur_value;
		++cnt;
	}
	return cnt;
}