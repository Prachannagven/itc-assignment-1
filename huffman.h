#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "global_req.h"
#include <stdio.h>
#include <stdlib.h>

void sort_nodes(node* arr[], int n);
void generate_codes(node* root, char* code, int depth);
void generate_huffman(node* sym_nodes, int sym_count);
float display_huffman_stats(node* sym_nodes, int sym_count);

#endif
