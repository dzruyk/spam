#ifndef __KEYWORD_H__
#define __KEYWORD_H__

typedef enum {
	KEYWORD_DEF,		//function definition
	KEYWORD_IF,
	KEYWORD_ELSE,
	KEYWORD_FOR,
	KEYWORD_DO,
	KEYWORD_WHILE,
	KEYWORD_UNKNOWN,
} keyword_t;

void keyword_table_init();

keyword_t keyword_table_lookup(char *name);

void keyword_table_destroy();

#endif

