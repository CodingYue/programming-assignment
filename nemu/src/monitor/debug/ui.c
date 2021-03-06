#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}
static int cmd_q(char *args) {
	return -1;
}
static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static int cmd_bt();

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "si [N] - Continue the execution of program with N steps", cmd_si},
	{ "info", "info [r/w] - Print some infomation about registers or watchpoints", cmd_info},
	{ "x", "x N EXPR - Print 4N bytes from EXPR", cmd_x},
	{ "p", "p EXPR - Evaluate value of EXPR", cmd_p},
	{ "w", "w EXPR - Program pause while EXPR changes", cmd_w},
	{ "d", "d N - Delete No.N watchpoint", cmd_d},
	{ "bt", "bt - Print Stack Frames", cmd_bt}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_x(char *args) {
	char *arg = strtok(NULL, " ");
	if (arg == NULL) return -2;
	int N = atoi(arg);

	arg = strtok(NULL, " ");
	if (arg == NULL) return -2;
	bool success;
	int EXPR = expr(arg, &success);

	int i;
	for (i = 0; i < N; ++i) {
		printf("0x%08x\n", swaddr_read(EXPR + 4*i, 4));
	}

	return 0;
}

static int cmd_d(char *args) {
	if (args == NULL) return -2;
	int NO = atoi(args);
	int result = free_wp(NO);
	if (result == -1) {
		printf("Nth watchpoint dosen't exist");
	}
	return 0;
}

static int cmd_w(char *args) {	
	if (args == NULL) return -2;
	bool success = false;
	int result = expr(args, &success);
	if (!success) {
		printf("Invalid EXPR %s\n", args);
	} else {
		new_wp(args, result);
	}
	return 0;
}

static int cmd_p(char *args) {
	if (args == NULL) return -2;
	bool success;
	int result = expr(args, &success);
	if (success == false) {
		printf("Invalid EXPR %s\n", args);
	} else {
		printf("%d\n", result);
	}
	return 0;
}

static int cmd_info(char *args) {
	char *arg = strtok(NULL, " ");
	int R;

	if (arg == NULL) return -2;

	if (strcmp(arg, "r") == 0) {
		for (R = 0; R < 8; ++R) printf("%s 0x%08x\n", regsl[R], cpu.gpr[R]._32);
		printf("eip 0x%08x\n", cpu.eip);
		return 0;
	}
	if (strcmp(arg, "w") == 0) {
		show_watchpoints();
		return 0;
	}
	return -2;
}

static int cmd_si(char *args) {
	char *arg = strtok(NULL, " ");

	int steps = 0;
	if (arg == NULL) {
		steps = 1;
	} else {
		steps = atoi(arg);
	}
	cpu_exec(steps);
	return 0;
}

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_bt() {
	swaddr_t ebp = cpu.ebp;
	while (ebp) {

	//	printf("%x\n", (int) ebp);
		swaddr_t addr = swaddr_read(ebp+4, 4);
		char *name = func_name(addr);
		printf("addr = 0x%x, %s()\n", addr, name);
		ebp = swaddr_read(ebp, 4);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		if (str == NULL) break;
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				int result = cmd_table[i].handler(args);
				if (result == -1) { return; }
				if (result == -2) { printf("Invalid arguments\n"); }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
