#ifndef FANO_H
#define FANO_H

#include "global_req.h"

void generate_fano(node* sym_nodes, int sym_count);
float display_fano_stats(node* sym_nodes, int sym_count);
void gen_fano_bitstream(char* bitstream, char* input_str, int str_len, int sym_num, node* sym_nodes);
void decode_fano_bitstream(char* bitstream, int sym_num, node* sym_nodes);

#endif
