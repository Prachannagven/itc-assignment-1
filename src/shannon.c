#include "shannon.h"
#include <math.h>
#include <string.h>

static int cmp_prob_desc(const void* a, const void* b) {
    const node* na = *(const node**)a;
    const node* nb = *(const node**)b;
    if (nb->prob > na->prob) return 1;
    if (nb->prob < na->prob) return -1;
    return 0;
}

static void float_to_bits(float val, int len, char* out) {
    for (int i = 0; i < len; i++) {
        val *= 2.0f;
        if (val >= 1.0f) {
            out[i] = '1';
            val -= 1.0f;
        } else {
            out[i] = '0';
        }
    }
    out[len] = '\0';
}

void generate_shannon(node* sym_nodes, int sym_count) {
    // Build sorted pointer array (descending probability)
    node* sorted[sym_count];
    for (int i = 0; i < sym_count; i++) sorted[i] = &sym_nodes[i];
    qsort(sorted, sym_count, sizeof(node*), cmp_prob_desc);

    float cumulative = 0.0f;
    for (int i = 0; i < sym_count; i++) {
        int len = (int)ceil(log2(1.0 / sorted[i]->prob));
        if (len < 1) len = 1;

        float_to_bits(cumulative, len, sorted[i]->sh_code);
        sorted[i]->sh_code_len = len;

        cumulative += sorted[i]->prob;
    }
}

float display_shannon_stats(node* sym_nodes, int sym_count) {
    float avg_len = 0;
    printf("Symbol | Probability | Shannon Code | Length\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%c      | %f    | %-20s| %d\n",
               sym_nodes[i].symbol, sym_nodes[i].prob,
               sym_nodes[i].sh_code, sym_nodes[i].sh_code_len);
        avg_len += sym_nodes[i].prob * sym_nodes[i].sh_code_len;
    }
    return avg_len;
}
