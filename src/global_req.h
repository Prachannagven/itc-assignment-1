#ifndef GLOBAL_REQ_H
#define GLOBAL_REQ_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_D 16  // maximum supported D-ary arity

typedef struct node {
    // Node Properties
    int id;
    float prob;
    char symbol;
    // Tree Creation Properties (generalised to D-ary)
    struct node* children[MAX_D];
    int child_count;  // number of children (0 for leaf nodes)
    // Huffman Code Values
    char code[100];
    int code_len;
    // Shannon-Type Code Values
    char st_code[100];
    int st_code_len;
    // Shannon Code Values
    char sh_code[100];
    int sh_code_len;
    // Fano Code Values
    char fano_code[100];
    int fano_code_len;
} node;

void free_tree(node* root);
float calculate_entropy(node* sym_nodes, int sym_count);

#endif
