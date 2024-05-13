#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <cstdio>

class Node{

public:
    Node(char data, int weight);
    Node *next_in_list;
    Node *prev_in_list;
    Node *left_tree;
    Node *right_tree;

    char data;
    int weight;
    bool is_weight_node_only;
};

class HuffmanListTree{
public:
    HuffmanListTree();
    void  add_to_list(char c, int weight);
    void  sort_list();
    Node* build_tree();
    Node* insert_node_in_list(Node *node); // make sure to do it in sorted order decending
    void detach_node_from_list(Node *n);
    void print();

private:
    Node *head;
    Node *last;


};


#endif // HUFFMAN_TREE_H