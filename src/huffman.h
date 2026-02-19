#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "global_req.h"
#include <stdio.h>
#include <stdlib.h>

void sort_nodes(node* arr[], int n);
void generate_codes(node* root, char* code, int depth, int D);
void generate_huffman(node* sym_nodes, int sym_count, int D);
float display_huffman_stats(node* sym_nodes, int sym_count);
float calc_huffman_efficiency(float huffman_len, float entropy);
void gen_huffman_bitstream(char* huffman_bitstream, char* input_str, int str_len, int sym_num,
                           node* sym_nodes);
void decode_huffman_bitstream(char* huffman_bitstream, int sym_num, node* sym_nodes);

#endif
