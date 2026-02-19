#include "global_req.h"
#include <math.h>
#include <stdlib.h>

void free_tree(node* root) {
    if (root == NULL)
        return;

    // Recurse over all children (works for binary and D-ary trees)
    for (int i = 0; i < root->child_count; i++) {
        free_tree(root->children[i]);
    }

    // Only free nodes that were dynamically allocated (internal nodes have id == -1)
    if (root->id == -1)
        free(root);
}

float calculate_entropy(node* sym_nodes, int sym_count) {
    float entropy = 0;
    for (int i = 0; i < sym_count; i++) {
        entropy += sym_nodes[i].prob * log2((double)sym_nodes[i].prob);
    }
    return -entropy;
}
