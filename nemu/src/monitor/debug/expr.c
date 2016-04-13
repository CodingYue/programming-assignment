#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	NOTYPE, MULTIPLE, DIVISION, EQUAL, PLUS, MINUS,
	DECIMAL, HEX, REG, L_brackets, R_brackets

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"==", EQUAL},						// equal
	{"\\+", PLUS},					// plus
	{"-", MINUS},
	{"\\*", MULTIPLE},
	{"/", DIVISION},
	{"0x[0-9]+", HEX},
	{"[0-9|A-F|a-f]+", DECIMAL},
	{"\\$[a-z]+", REG},
	{"\\(", L_brackets},
	{"\\)", R_brackets}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;


static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				if (rules[i].token_type == NOTYPE) break;
				tokens[nr_token].type = rules[i].token_type;
				strncpy(tokens[nr_token].str, substr_start, substr_len);
				tokens[nr_token++].str[substr_len] = '\0';

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

struct expr {
	int type;
	union {
		int oper;
		int value;
	};
};

struct expr suffix[32];
struct expr stack[32];

int expr(char *e, bool *success) {
	*success = false;
	if(!make_token(e)) {
		return 0;
	}

	int top = 0;
	int nr_suffix = 0;
	int i;

	for (i = 0; i < nr_token; ++i) {
		
		struct expr cur;

		if (tokens[i].type == HEX) {
			cur.type = 0;
			cur.value = (int) strtol(tokens[i].str, NULL, 16);
		} else
		if (tokens[i].type == DECIMAL) {
			cur.type = 0;
			cur.value = (int) strtol(tokens[i].str, NULL, 0);
		} else
		if (tokens[i].type == REG) {
			cur.type = 0;
			cur.value = get_reg(tokens[i].str+1);
		} else {
			cur.type = 1;
			cur.oper = tokens[i].type;
		}

		if (cur.type == 0) {
			suffix[nr_suffix++] = cur;
		} else {
			if (cur.oper == L_brackets) {
				stack[top++] = cur;
			} else
			if (cur.oper == R_brackets) {
				bool find = false;
				while (top > 0) {
					struct expr now = stack[--top];
					if (now.oper == L_brackets) {
						find = true;
						break;
					}
					suffix[nr_suffix++] = now;
				}
				if (!find) return 0;
			} else {
				while (top > 0) {
					struct expr now = stack[top-1];
					if (now.oper == L_brackets) break;
					if (cur.oper + 1 < now.oper) break;
					suffix[nr_suffix++] = now;
					--top;
				}
				stack[top++] = cur;
			}
		}
	}

	while (top > 0) {
		suffix[nr_suffix++] = stack[--top];
	}
	
	for (i = 0; i < nr_suffix; ++i) {
		if (suffix[i].type == 0) {
			stack[top++] = suffix[i];
		} else {
			if (top == 0) return 0;
			if (stack[top-1].type != 0) return 0;
			if (top == 1 || (top >= 2 && stack[top-2].type == 1)) {
				if (suffix[i].oper == MULTIPLE) {
					stack[top-1].value = swaddr_read(stack[top-1].value, 4);
				} else
				if (suffix[i].oper == MINUS) {
					stack[top-1].value *= -1;
				} else {
					return 0;
				}
			}
			if (top >= 2) {
				int y = stack[--top].value;
				int x = stack[--top].value;
				stack[top].type = 0;
				switch (suffix[i].oper) {
					case PLUS: stack[top++].value = x+y; break;
					case MINUS: stack[top++].value = x-y; break;
					case MULTIPLE: stack[top++].value = x*y; break;
					case DIVISION: stack[top++].value = x/y; break;
					default: return 0;
				};
			}
		}
	}
	if (top != 1) return 0;
	int result = stack[--top].value;
	*success = true;
	return result;
}

