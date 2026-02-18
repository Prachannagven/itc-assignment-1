#ifndef SHANNON_TYPE_H
#define SHANNON_TYPE_H

#include "global_req.h"

void generate_shannon_type(node* sym_nodes, int sym_count);
float display_shannon_type_stats(node* sym_nodes, int sym_count);
void gen_shannon_type_bitstream(char* bitstream, char* input_str, int str_len, int sym_num, node* sym_nodes);
void decode_shannon_type_bitstream(char* bitstream, int sym_num, node* sym_nodes);

#endif
