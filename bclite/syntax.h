#ifndef _SYNTAX_H_
#define _SYNTAX_H_

#include "syn_tree.h"

syn_tree_t *program_start();

syn_tree_t *statesment();

syn_tree_t *expr();

syn_tree_t *expr_rest();

syn_tree_t *term();

syn_tree_t *term_rest();

syn_tree_t *factor();

#endif

