#ifndef __TRAVERSE_H__
#define __TRAVERSE_H__

#include "common.h"
#include "syn_tree.h"

ret_t traverse_prog(syn_node_t *tree);

//pop all items from stack and print it
void traverse_result();

#endif

