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
 * * v0.2 - Takes a symbol in and then performs huffman encoding. Added in entropy and avg. length
 * calculation of huffman
 */
#include "global_req.h"
#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Incorrect usage. This executable takes in no arguuments.\nExiting ... \n");
        return -1;
    }

    int sym_num = 0;

    // Querying the Numer of Symbols from the User
    printf("Please enter the number of symbols: ");
    scanf("%d", &sym_num);
    if (sym_num < 2) {
        printf("More than 1 symbol is required to make codes. \nExiting...\n");
        return -1;
    }
    float sym_prob[sym_num];
    char sym_char[sym_num];
    node sym_nodes[sym_num];
    float sym_prob_sum = 0;

    printf("Please enter the probabilities for the %d symbols: \n", sym_num);
    for (int i = 0; i < sym_num; i++) {
        printf("Enter probability and character for symbol p_%d: ", i);
        if (scanf(" %c:%f", &sym_char[i], &sym_prob[i]) != 2) {
            printf("Invalid input format. Use: A:0.25\nExiting...\n");
            return -1;
        }
        sym_prob_sum += sym_prob[i];
        if (sym_prob_sum > 1) {
            printf("Net probability has exceeded 1. Something is wrong. \nExtiing...\n");
            return -1;
        }
    }

    // Assigning all of the probabilities and symbols into nodes
    for (int i = 0; i < sym_num; i++) {
        sym_nodes[i].id = i;
        sym_nodes[i].symbol = sym_char[i];
        sym_nodes[i].data = sym_prob[i];
        sym_nodes[i].left = NULL;
        sym_nodes[i].right = NULL;
    }

    generate_huffman(sym_nodes, sym_num);
    float huffman_avg_len = display_huffman_stats(sym_nodes, sym_num);
    printf("Average Huffman Code Length = %f\n", huffman_avg_len);

    return 0;
}
