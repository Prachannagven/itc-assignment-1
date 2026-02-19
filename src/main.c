/*
 *
 * Author: Pranav Chandra N. V.
 * Course; ECE F344 - Information Theory and Coding
 * Project: Assignment 1
 * Date: 18/02/2026
 * Description: My solution to the ITC assignment posted. Requests probability inputs from user,
 * then performs Huffman, Shanon-Type, Shanon and Fano Coding before returning to user with
 * statistics.
 * Version: v0.2
 * Version History:
 * * v0.1 - 17/02/2026 - First version. Performs symbol blind huffman encoding using a binary tree
 * * v0.2 - 18/02/2026 - Takes a symbol in and then performs huffman encoding. Added in entropy and
 * avg. length calculation of huffman
 * * v0.3 - 18/02/2026 - Takes a string as an input directly, and does string parsing to set up the
 * nodes.
 * * v1.0 - 18/02/2026 - Implements shanon and shanon fano type code
 * * v1.1 - 18/02/2026 - Generates the bitstream of the different types of code and fixed up the
 * additional code files. Also had to fix the makefile.
 * * v1.2 - 18/02/2026 - Minor fixes with regards to printing of encoded values.
 * * v1.3 - 19/02/2026 - Now can perform D-ary encoding for huffman. Default tree size is now
 * expanded to allow it
 */
#include "fano.h"
#include "global_req.h"
#include "huffman.h"
#include "shannon.h"
#include "shannon_type.h"
#include <stdio.h>
#include <string.h>

#define MAX_STRING_SIZE 10000

int sym_num = 0;

int main(int argc, char* argv[]) {
    /* Ensuring Command has been run properly */
    if (argc == 1) {
        printf("You have not added an additional file. You will be prompted to enter a string\n");
    } else if (argc > 2) {
        printf("Invalid usage. You may either run the program, or attach a file to be encoded.\n");
        return -1;
    }

    /* Taking a string input from the user */
    char input_str[MAX_STRING_SIZE];
    int str_len = 0;
    if (argc == 1) {
        printf("Please enter a string to encode (max 255 chars): ");
        fgets(input_str, sizeof(input_str), stdin);
        input_str[strcspn(input_str, "\n")] = '\0';
        str_len = strlen(input_str);

        if (str_len < 2) {
            printf("String must be at least 2 characters long.\nExiting...\n");
            return -1;
        }
    } else if (argc == 2) {
        FILE* fp = fopen(argv[1], "r");
        if (fp == NULL) {
            printf("Could not open file: %s\nExiting...\n", argv[1]);
            return -1;
        }
        int c;
        while (str_len < (int)(sizeof(input_str) - 1) && (c = fgetc(fp)) != EOF) {
            input_str[str_len++] = (char)c;
        }
        fclose(fp);
        input_str[str_len] = '\0';
        if (str_len < 2) {
            printf("File string must be at least 2 characters long.\nExiting...\n");
            return -1;
        }
    }

    /* Computing symbol frequencies and probabilities from the string */
    int freq[256] = {0};
    for (int i = 0; i < str_len; i++) {
        freq[(unsigned char)input_str[i]]++;
    }

    // Count unique symbols
    sym_num = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            sym_num++;
        }
    }

    // Assigning the symbols into nodes
    node sym_nodes[sym_num];
    int idx = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            sym_nodes[idx].id = idx;
            sym_nodes[idx].symbol = (char)i;
            sym_nodes[idx].prob = (float)freq[i] / str_len;
            sym_nodes[idx].child_count = 0; // leaf node: no children
            idx++;
        }
    }

    /* Entropy Calculation */
    float entropy = calculate_entropy(sym_nodes, sym_num);
    printf("\nEntropy H = %f bits/symbol\n", entropy);

    /* Huffman Encoding */
    // Ask for arity (D=2 gives standard binary Huffman)
    int D = 2;
    printf("\nEnter D for D-ary Huffman encoding (2 = binary, 3 = ternary, ...): ");
    scanf("%d", &D);
    if (D < 2) {
        printf("D must be >= 2. Defaulting to binary (D=2).\n");
        D = 2;
    }
    if (D > sym_num) {
        printf("D (%d) > number of symbols (%d). Defaulting to binary (D=2).\n", D, sym_num);
        D = 2;
    }

    // Encoding and Displaying the Symbols
    printf("\n=== %d-ary Huffman Code ===\n", D);
    generate_huffman(sym_nodes, sym_num, D);
    float huffman_avg_len = display_huffman_stats(sym_nodes, sym_num);
    printf("Average Length = %f | Efficiency = %.2f%%\n", huffman_avg_len,
           calc_huffman_efficiency(huffman_avg_len, entropy) * 100.0f);

    // Generating and printing the bitstream
    char huffman_bitstream[str_len * 100];
    gen_huffman_bitstream(huffman_bitstream, input_str, str_len, sym_num, sym_nodes);
    printf("Huffman bitstream:\n%s", huffman_bitstream);
    decode_huffman_bitstream(huffman_bitstream, sym_num, sym_nodes);

    /* Shanon-Type Coding */
    printf("\n=== Shannon-Type Code ===\n");
    generate_shannon_type(sym_nodes, sym_num);
    float st_avg_len = display_shannon_type_stats(sym_nodes, sym_num);
    printf("Average Length = %f | Efficiency = %.2f%%\n", st_avg_len,
           (entropy / st_avg_len) * 100.0f);
    char st_bitstream[str_len * 100];
    gen_shannon_type_bitstream(st_bitstream, input_str, str_len, sym_num, sym_nodes);
    printf("Shannon-Type bitstream:\n%s", st_bitstream);
    decode_shannon_type_bitstream(st_bitstream, sym_num, sym_nodes);

    /* Shanon Coding */
    printf("\n=== Shannon Code ===\n");
    generate_shannon(sym_nodes, sym_num);
    float sh_avg_len = display_shannon_stats(sym_nodes, sym_num);
    printf("Average Length = %f | Efficiency = %.2f%%\n", sh_avg_len,
           (entropy / sh_avg_len) * 100.0f);
    char sh_bitstream[str_len * 100];
    gen_shannon_bitstream(sh_bitstream, input_str, str_len, sym_num, sym_nodes);
    printf("Shannon bitstream:\n%s", sh_bitstream);
    decode_shannon_bitstream(sh_bitstream, sym_num, sym_nodes);

    /* Shanon-Fano Coding */
    printf("\n=== Shannon-Fano Code ===\n");
    generate_fano(sym_nodes, sym_num);
    float fano_avg_len = display_fano_stats(sym_nodes, sym_num);
    printf("Average Length = %f | Efficiency = %.2f%%\n", fano_avg_len,
           (entropy / fano_avg_len) * 100.0f);
    char fano_bitstream[str_len * 100];
    gen_fano_bitstream(fano_bitstream, input_str, str_len, sym_num, sym_nodes);
    printf("Fano bitstream:\n%s", fano_bitstream);
    decode_fano_bitstream(fano_bitstream, sym_num, sym_nodes);

    /* --- Comparison Table --- */
    printf("\n=== Comparison Summary ===\n");
    printf("%-20s | %s\n", "Coding Scheme", "Avg Length");
    printf("%-20s | %f\n", "Entropy Bound", entropy);
    printf("%-20s | %f\n", "Huffman", huffman_avg_len);
    printf("%-20s | %f\n", "Shannon-Type", st_avg_len);
    printf("%-20s | %f\n", "Shannon", sh_avg_len);
    printf("%-20s | %f\n", "Shannon-Fano", fano_avg_len);

    return 0;
}
