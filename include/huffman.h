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
    HuffmanCompress();
    uint8_t compress();
    void find_char_freq();
    //uint8_t* get_huffman_codes();
    void print_map();
    string decompress(vector<tuple<char, uint8_t>> huff_codes, uint8_t *data);
    HuffmanListTree get_huffman_list_tree() {return this->htree; }
    vector<tuple<char, uint8_t>> get_huffman_codes() { return this->huff_codes; }


private:
    map<char, size_t> char_to_freq;
    string string_to_compress;
    HuffmanListTree htree;
    vector<tuple<char, uint8_t>> huff_codes;

};


#endif