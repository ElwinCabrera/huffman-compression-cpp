#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <unordered_map>
//#include <map>
#include <cstdint>
#include <string>
#include <tuple>



#include "huffman_tree.h"
#include "bits.h"

//using namespace std;
using std::string;
//using std::map;
using std::unordered_map;
//using std::vector;
using std::cout;
using std::endl;
using std::to_string;
using std::stoi;
using std::tuple;

class HuffmanCompress {
public:
    HuffmanCompress();
    HuffmanCompress(string s);
    HuffmanCompress(char *data, int len);
    ~HuffmanCompress();
    void init(char *data, int data_len);
    
    uint8_t* compress(); // probably good idea to also return the length
    //tuple<uint8_t*, size_t> compress(); // probably good idea to also return the length
    uint8_t* decompress(uint8_t *data, int len);
    void print_stats();
    

    HuffmanListTree get_huffman_list_tree() {return this->htree; }
    unordered_map<char, BitSequence> get_huffman_codes() { return this->huff_codes; }

    size_t get_compressed_len(){
        this->compressed_total_byte_len = this->compressed_total_bit_len / BYTE_LEN;
        if(this->compressed_total_bit_len % BYTE_LEN != 0) this->compressed_total_byte_len += 1;
        return this->compressed_total_byte_len;
    }


protected:
    
    shared_ptr<uint8_t> serialize_huff_codes();
    unordered_map<char, BitSequence> deserialize_huff_codes(uint8_t *data);
    shared_ptr<BitSequence> create_bit_seq(uint8_t *data, int length);
    void gen_byte_freq_map();
    void print_byte_freq_map();
    


    uint8_t *uncompressed_data;
    uint8_t *compressed_data;

    shared_ptr<BitSequence> compressed_str_bitsequence;
    //BitSequence huff_codes_bitsequence;
    shared_ptr<uint8_t> serialized_huff_codes;

    unordered_map<uint, size_t> byte_freq_map;
    unordered_map<char, BitSequence> huff_codes;

    HuffmanListTree htree;

    size_t uncompressed_data_len;
    size_t uncompressed_data_bit_len;
    size_t huff_code_bit_len;

    size_t serialized_huff_code_bit_len;  //since we are serializing as string this will always be a multiple of 8
    size_t serialized_huff_code_byte_len;

    size_t compressed_str_bit_len;

    size_t compressed_total_bit_len; // = serialized_huff_code_bit_len + compressed_str_bit_len
    size_t compressed_total_byte_len;
    

};


#endif