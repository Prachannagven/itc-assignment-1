#include "fano.h"
#include <string.h>

static int cmp_prob_desc(const void* a, const void* b) {
    const node* na = *(const node**)a;
    const node* nb = *(const node**)b;
    if (nb->prob > na->prob)
        return 1;
    if (nb->prob < na->prob)
        return -1;
    return 0;
}

static void fano_split(node** sorted, int start, int end, char* prefix, int depth) {
    if (start >= end) {
        // Single symbol - assign the accumulated prefix as its code
        prefix[depth] = '\0';
        strcpy(sorted[start]->fano_code, prefix);
        sorted[start]->fano_code_len = depth;
        return;
    }

    // Compute total probability of the range
    float total = 0.0f;
    for (int i = start; i <= end; i++)
        total += sorted[i]->prob;

    // Find the split point that minimises the imbalance between the two halves
    float running = 0.0f;
    int split = start;
    float best_diff = total; // worst-case starting value
    for (int i = start; i < end; i++) {
        running += sorted[i]->prob;
        float diff = running - (total - running);
        if (diff < 0)
            diff = -diff;
        if (diff < best_diff) {
            best_diff = diff;
            split = i;
        }
    }

    // Top half gets '0', bottom half gets '1'
    prefix[depth] = '0';
    fano_split(sorted, start, split, prefix, depth + 1);

    prefix[depth] = '1';
    fano_split(sorted, split + 1, end, prefix, depth + 1);
}

void generate_fano(node* sym_nodes, int sym_count) {
    node* sorted[sym_count];
    for (int i = 0; i < sym_count; i++)
        sorted[i] = &sym_nodes[i];
    qsort(sorted, sym_count, sizeof(node*), cmp_prob_desc);

    char prefix[100];
    fano_split(sorted, 0, sym_count - 1, prefix, 0);
}

float display_fano_stats(node* sym_nodes, int sym_count) {
    float avg_len = 0;
    printf("Symbol | Probability | Shannon-Fano Code | Length\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%c      | %f    | %-18s| %d\n", sym_nodes[i].symbol, sym_nodes[i].prob,
               sym_nodes[i].fano_code, sym_nodes[i].fano_code_len);
        avg_len += sym_nodes[i].prob * sym_nodes[i].fano_code_len;
    }
    return avg_len;
}

void gen_fano_bitstream(char* bitstream, char* input_str, int str_len, int sym_num,
                        node* sym_nodes) {
    int idx = 0;
    for (int i = 0; i < str_len; i++) {
        for (int j = 0; j < sym_num; j++) {
            if (input_str[i] == sym_nodes[j].symbol) {
                for (int k = 0; k < sym_nodes[j].fano_code_len; k++) {
                    bitstream[idx++] = sym_nodes[j].fano_code[k];
                }
            }
        }
    }
    bitstream[idx++] = '\n';
    bitstream[idx] = '\0';
}

void decode_fano_bitstream(char* bitstream, int sym_num, node* sym_nodes) {
    printf("Fano decoded: ");
    int i = 0;
    while (bitstream[i] != '\0' && bitstream[i] != '\n') {
        for (int j = 0; j < sym_num; j++) {
            int match = 1;
            for (int k = 0; k < sym_nodes[j].fano_code_len; k++) {
                if (bitstream[i + k] != sym_nodes[j].fano_code[k]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                printf("%c", sym_nodes[j].symbol);
                i += sym_nodes[j].fano_code_len;
                break;
            }
        }
    }
    printf("\n");
}
