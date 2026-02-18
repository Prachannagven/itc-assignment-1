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

int sym_num = 0;
float sym_prob_sum = 0;
char encoding_string[256];

int main(int argc, char* argv[]) {
    /* Ensuring Command has been run properly */
    if (argc != 1) {
        printf("Incorrect usage. This executable takes in no arguuments.\nExiting ... \n");
        return -1;
    }

    /* Taking User inputs for symbols and probabilities */
    printf("Please enter the number of symbols: ");
    scanf("%d", &sym_num);
    if (sym_num < 2) {
        printf("More than 1 symbol is required to make codes. \nExiting...\n");
        return -1;
    }
    float sym_prob[sym_num];
    char sym_char[sym_num];
    node sym_nodes[sym_num];

    printf("Please enter the probabilities for the %d symbols: \n", sym_num);
    for (int i = 0; i < sym_num; i++) {
        printf("\tEnter probability and character for symbol p_%d: ", i);
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

    for (int i = 0; i < sym_num; i++) {
        sym_nodes[i].id = i;
        sym_nodes[i].symbol = sym_char[i];
        sym_nodes[i].prob = sym_prob[i];
        sym_nodes[i].left = NULL;
        sym_nodes[i].right = NULL;
    }

    /*Entropy Calculation */
    float entropy = calculate_entropy(sym_nodes, sym_num);

    /* Performing Huffman Encoding and Printing the Results */
    generate_huffman(sym_nodes, sym_num);
    float huffman_avg_len = display_huffman_stats(sym_nodes, sym_num);
    printf("Entropy for these symbols is  = %f\n", entropy);
    printf("Average Huffman Code Length = %f\n", huffman_avg_len);

    /* Coding Efficiency */
    float huffman_effic = calc_huffman_efficiency(huffman_avg_len, entropy);
    printf("Huffman Coding Efficiency is = %f%\n", huffman_effic * 100.0);
    return 0;
}
