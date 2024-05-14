#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <map>
#include <cstdint>
#include <string>

#include "../include/huffman_tree.h"

using namespace std;

class HuffmanCompress {
public:
    HuffmanCompress(string s);
    uint8_t compress();
    void find_char_freq();
    void print_map();
    string decompress();
    HuffmanListTree get_huffman_list_tree() {return this->htree; }


private:
    map<char, size_t> char_to_freq;
    string string_to_compress;
    HuffmanListTree htree;

};


#endif