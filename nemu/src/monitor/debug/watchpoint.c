#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <string.h>



#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
		wp_list[i].expr = NULL;
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

void new_wp(char *e, int value) {
	Assert(free_ != NULL, "WP overflow!");
	WP *wp = free_;
	
	int len = strlen(e);
	wp->expr = (char *) malloc((len+1) * sizeof(char));
	strcpy(wp->expr, e);

	wp->value = value;
	free_ = free_->next;
	wp->next = head;
	head = wp;
}

int free_wp(int NO) {
	WP *wp = &wp_list[NO];
	if (wp->expr == NULL) return -1;
	wp->expr = NULL;
	wp->next = free_;
	free_ = wp;
	return 0;
}

void show_watchpoints() {
	int i;
	for (i = 0; i < NR_WP; ++i) {
		if (wp_list[i].expr == NULL) continue;
		printf("No.%d, expr is %s, expr value is %d\n", i, wp_list[i].expr, wp_list[i].value);
	}
}
