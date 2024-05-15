#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <unordered_map>
//#include <map>
#include <tuple>
#include <cstdint>

#include "bits.h"

using std::unordered_map;
//using std::map;

class Node{

public:
    Node(char data, int weight, bool ignore_data = false);
    Node *next_in_list;
    Node *prev_in_list;
    Node *left_tree;
    Node *right_tree;

    char data;
    int weight;
    bool ignore_data;
};

class HuffmanListTree {
public:
    HuffmanListTree();
    ~HuffmanListTree();

    void add_to_list(char c, int weight);
    Node* insert_node_in_list(Node *node); // make sure to do it in sorted order decending
    void detach_node_from_list(Node *n);
    void sort_list();
    void print_list();
    void free_list();

    Node* build_huffman_tree();
    void recreate_huffman_tree(unordered_map<char, BitSequence> codes);
    void recreate_huff_tree_helper(Node *curr, char data, BitSequence bit_sequence, uint8_t bit_idx);
    void print_tree(Node *curr_head);
    void free_tree(Node *curr_head);

    unordered_map<char, BitSequence> generate_huffman_codes();
    void gen_huff_code_helper(Node *curr_head, uint8_t depth, uint8_t curr_code, bool set);
    void print_huff_codes();
    uint8_t get_sig_digs_in_code(uint8_t code);

    
    
    Node* get_head() {return this->head;}
   
private:
    Node *head;
    unordered_map<char, BitSequence> huff_codes;
    size_t tree_depth;


};


#endif // HUFFMAN_TREE_H