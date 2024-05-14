#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cstdint>

using std::unordered_map;
using std::tuple;
using std::vector;
using std::tie;

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

class HuffmanListTree{
public:
    HuffmanListTree();
    ~HuffmanListTree();
    void  add_to_list(char c, int weight);
    void  sort_list();
    Node* build_huffman_tree();
    Node* insert_node_in_list(Node *node); // make sure to do it in sorted order decending
    void detach_node_from_list(Node *n);
    void print_list();
    void print_tree(Node *curr_head);
    vector<tuple<char, uint8_t>> generate_huffman_codes();
    void gen_huff_code_helper(Node *curr_head, uint8_t curr_code, uint8_t code);
    void recreate_huffman_tree(vector<tuple<char, uint8_t>> codes);
    void recreate_huff_tree_helper(Node *curr, char data, uint8_t code, uint8_t bit_idx);
    uint8_t get_sig_digs_in_code(uint8_t code);
    void print_huff_codes();
    void free_tree(Node *curr_head);
    void free_list();
    Node* get_head() {return this->head;}
   

private:
    Node *head;
    Node *last;
    vector<tuple<char, uint8_t>> huff_codes;


};


#endif // HUFFMAN_TREE_H