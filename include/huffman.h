#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <cstdint>
#include <string>

#include "huffman_tree.h"
#include "bits.h"

using namespace std;
using std::string;
using std::map;
using std::unordered_map;
using std::vector;

class HuffmanCompress {
public:
    HuffmanCompress(string s);
    HuffmanCompress();
    ~HuffmanCompress();
    BitSequence compress();
    void find_char_freq();
    void print_map();
    string decompress(unordered_map<char, BitSequence> huff_codes); //testing decompres function
    //string decompress(uint8_t *data); //real decompress function
    HuffmanListTree get_huffman_list_tree() {return this->htree; }
    unordered_map<char, BitSequence> get_huffman_codes() { return this->huff_codes; }


private:
    string string_to_compress;
    BitSequence compressed_bitsequence;
    BitSequence huff_codes_bitsequence;

    map<char, size_t> char_to_freq;
    unordered_map<char, BitSequence> huff_codes;

    HuffmanListTree htree;

    size_t num_bits_in_huff_code;
    size_t num_bits_in_compressed_str;
    size_t num_bits_in_str;

};


#endif