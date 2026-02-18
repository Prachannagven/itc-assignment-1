#include "shannon_type.h"
#include <math.h>
#include <string.h>

static int cmp_prob_desc(const void* a, const void* b) {
    const node* na = *(const node**)a;
    const node* nb = *(const node**)b;
    if (nb->prob > na->prob) return 1;
    if (nb->prob < na->prob) return -1;
    return 0;
}

static void int_to_bits(int val, int len, char* out) {
    for (int i = len - 1; i >= 0; i--) {
        out[i] = (char)('0' + (val & 1));
        val >>= 1;
    }
    out[len] = '\0';
}

void generate_shannon_type(node* sym_nodes, int sym_count) {
    // Build a sorted pointer array (descending probability)
    node* sorted[sym_count];
    for (int i = 0; i < sym_count; i++) sorted[i] = &sym_nodes[i];
    qsort(sorted, sym_count, sizeof(node*), cmp_prob_desc);

    // Compute lengths
    int lengths[sym_count];
    for (int i = 0; i < sym_count; i++) {
        lengths[i] = (int)ceil(log2(1.0 / sorted[i]->prob));
        if (lengths[i] < 1) lengths[i] = 1;
    }

    // Canonical code assignment
    int code_val = 0;
    int prev_len = lengths[0];
    for (int i = 0; i < sym_count; i++) {
        // Shift left by the difference in length from previous symbol
        code_val <<= (lengths[i] - prev_len);
        prev_len    = lengths[i];

        int_to_bits(code_val, lengths[i], sorted[i]->st_code);
        sorted[i]->st_code_len = lengths[i];
        code_val++;
    }
}

float display_shannon_type_stats(node* sym_nodes, int sym_count) {
    float avg_len = 0;
    printf("Symbol | Probability | Shannon-Type Code | Length\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%c      | %f    | %-20s| %d\n",
               sym_nodes[i].symbol, sym_nodes[i].prob,
               sym_nodes[i].st_code, sym_nodes[i].st_code_len);
        avg_len += sym_nodes[i].prob * sym_nodes[i].st_code_len;
    }
    return avg_len;
}
