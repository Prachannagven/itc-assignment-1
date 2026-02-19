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

void generate_codes(node* root, char* code, int depth, int D) {
    if (root->child_count == 0) {
        // Leaf node: assign code only for main symbols (internal nodes have -1 and dumies have -2)
        code[depth] = '\0';
        if (root->id >= 0) {
            strcpy(root->code, code);
            root->code_len = depth;
        }
        return;
    }
    for (int i = 0; i < D; i++) {
        if (root->children[i]) {
            code[depth] = (char)('0' + i);
            generate_codes(root->children[i], code, depth + 1, D);
        }
    }
}

void generate_huffman(node* sym_nodes, int sym_count, int D) {
    int remainder = (sym_count - 1) % (D - 1);
    int dummies   = (remainder == 0) ? 0 : (D - 1 - remainder);
    int total     = sym_count + dummies;

    node dummy_store[D - 1];
    for (int i = 0; i < dummies; i++) {
        dummy_store[i].id          = -2;   // marker: dummy
        dummy_store[i].prob        = 0.0f;
        dummy_store[i].symbol      = 0;
        dummy_store[i].child_count = 0;
        dummy_store[i].code[0]     = '\0';
        dummy_store[i].code_len    = 0;
    }

    node* active[total];
    for (int i = 0; i < dummies; i++)
        active[i] = &dummy_store[i];
    for (int i = 0; i < sym_count; i++)
        active[dummies + i] = &sym_nodes[i];
    int active_count = total;

    while (active_count > 1) {
        sort_nodes(active, active_count);

        node* parent = malloc(sizeof(node));
        parent->id          = -1;
        parent->prob        = 0.0f;
        parent->child_count = D;

        for (int i = 0; i < D; i++) {
            parent->children[i]  = active[i];
            parent->prob        += active[i]->prob;
        }

        int remaining = active_count - D;
        for (int i = 0; i < remaining; i++)
            active[i] = active[D + i];
        active[remaining] = parent;
        active_count      = remaining + 1;
    }

    char code_buf[200];
    generate_codes(active[0], code_buf, 0, D);
    free_tree(active[0]);
}

float display_huffman_stats(node* sym_nodes, int sym_count) {
    float huffman_avg_len = 0;
    printf("Symbol | Probability | Huffman Code | Length\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%c      | %f    | %-9s    | %d\n", sym_nodes[i].symbol, sym_nodes[i].prob,
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
