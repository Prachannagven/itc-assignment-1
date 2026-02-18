#include "huffman.h"
#include "global_req.h"
#include <stdlib.h>
#include <string.h>

void sort_nodes(node* arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j]->prob > arr[j + 1]->prob) {
                node* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void generate_codes(node* root, char* code, int depth) {
    if (root->left == NULL && root->right == NULL) {
        code[depth] = '\0';
        // printf("Symbol %d -> %s \n", root->id, code);
        strcpy(root->code, code);
        root->code_len = depth;
        return;
    }

    if (root->left) {
        code[depth] = '0';
        generate_codes(root->left, code, depth + 1);
    }

    if (root->right) {
        code[depth] = '1';
        generate_codes(root->right, code, depth + 1);
    }
}

void generate_huffman(node* sym_nodes, int sym_count) {
    node* active_nodes[sym_count];
    node* root;

    // Step 2 - Creating a set of nodes to play around with while making the tree
    for (int i = 0; i < sym_count; i++) {
        active_nodes[i] = &sym_nodes[i];
    }
    int active_count = sym_count;

    // Step 3 - Constantly sorting through and adding the nodes with the lowest prob values until
    // the tree reaches one node
    while (active_count > 1) {
        sort_nodes(active_nodes, active_count);

        node* left = active_nodes[0];
        node* right = active_nodes[1];

        node* parent = malloc(sizeof(node));
        parent->id = -1;
        parent->prob = left->prob + right->prob;
        parent->right = right;
        parent->left = left;

        for (int i = 2; i < active_count; i++) {
            active_nodes[i - 2] = active_nodes[i];
        }

        active_count -= 2; // removed 2 nodes
        active_nodes[active_count] = parent;
        active_count++; // added 1 node
    }

    root = active_nodes[0];

    char code_buf[100];
    generate_codes(root, code_buf, 0);

    free_tree(root);
}

float display_huffman_stats(node* sym_nodes, int sym_count) {
    float huffman_avg_len = 0;
    printf("Symbol | Probability | Huffman Code | Length\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%c      | %f    | %s    | %d\n", sym_nodes[i].symbol, sym_nodes[i].prob,
               sym_nodes[i].code, sym_nodes[i].code_len);
        huffman_avg_len += sym_nodes[i].prob * sym_nodes[i].code_len;
    }
    return huffman_avg_len;
}

float calc_huffman_efficiency(float huffman_len, float entropy) {
    float huffman_effic = (entropy / huffman_len);
    return huffman_effic;
}

void gen_huffman_bitstream(char* huffman_bitstream, char* input_str, int str_len, int sym_num,
                           node* sym_nodes) {
    int idx = 0;
    for (int i = 0; i < str_len; i++) {
        for (int j = 0; j < sym_num; j++) {
            if (input_str[i] == sym_nodes[j].symbol) {
                for (int k = 0; k < sym_nodes[j].code_len; k++) {
                    huffman_bitstream[idx] = sym_nodes[j].code[k];
                    idx++;
                }
            }
        }
    }
    huffman_bitstream[idx] = '\n';
    huffman_bitstream[idx + 1] = '\0';
}

void decode_huffman_bitstream(char* huffman_bitstream, int sym_num, node* sym_nodes) {
    printf("Decoded string: ");
    int i = 0;
    while (huffman_bitstream[i] != '\0' && huffman_bitstream[i] != '\n') {
        for (int j = 0; j < sym_num; j++) {
            int match = 1;
            for (int k = 0; k < sym_nodes[j].code_len; k++) {
                if (huffman_bitstream[i + k] != sym_nodes[j].code[k]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                printf("%c", sym_nodes[j].symbol);
                i += sym_nodes[j].code_len;
                break;
            }
        }
    }
    printf("\n");
}
