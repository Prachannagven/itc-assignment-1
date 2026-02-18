#ifndef GLOBAL_REQ_H
#define GLOBAL_REQ_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    // Node Properties
    int id;
    float prob;
    char symbol;
    // Tree Creation Properties
    struct node* left;
    struct node* right;
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
