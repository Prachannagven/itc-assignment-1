#include "global_req.h"
#include <stdlib.h>

void free_tree(node* root) {
    if (root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);

    // Only free nodes that were dynamically allocated
    if (root->id == -1)
        free(root);
}
