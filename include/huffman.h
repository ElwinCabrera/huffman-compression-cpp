#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <cstdint>
#include <string>


#include "huffman_tree.h"
#include "bits.h"

//using namespace std;
using std::string;
using std::map;
using std::unordered_map;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;
using std::stoi;

class HuffmanCompress {
public:
    HuffmanCompress(string s);
    //HuffmanCompress(char *data, size_t len);
    HuffmanCompress();
    ~HuffmanCompress();
    uint8_t* compress();
    uint8_t* serialize_huff_codes();
    unordered_map<char, BitSequence> deserialize_huff_codes(uint8_t *data);
    void find_char_freq();
    void print_map();
    //string decompress(unordered_map<char, BitSequence> huff_codes); //testing decompres function
    string decompress(uint8_t *data, int len); //real decompress function
    BitSequence create_bit_seq(uint8_t *data, int length);
    void print_stats();
    HuffmanListTree get_huffman_list_tree() {return this->htree; }
    unordered_map<char, BitSequence> get_huffman_codes() { return this->huff_codes; }

    size_t get_compressed_len(){
        this->compressed_total_byte_len = this->compressed_total_bit_len / BYTE_LEN;
        if(this->compressed_total_bit_len % BYTE_LEN != 0) this->compressed_total_byte_len += 1;
        return this->compressed_total_byte_len;
    }


private:
    string uncompressed_str;
    uint8_t *compressed_data;
    BitSequence compressed_str_bitsequence;
    //BitSequence huff_codes_bitsequence;
    uint8_t *serialized_huff_codes;

    map<char, size_t> char_to_freq;
    unordered_map<char, BitSequence> huff_codes;

    HuffmanListTree htree;


    size_t uncompressed_str_bit_len;
    size_t huff_code_bit_len;

    size_t serialized_huff_code_bit_len;  //since we are serializing as string this will always be a multiple of 8
    size_t serialized_huff_code_byte_len;

    size_t compressed_str_bit_len;

    size_t compressed_total_bit_len; // = serialized_huff_code_bit_len + compressed_str_bit_len
    size_t compressed_total_byte_len;
    

};


#endif