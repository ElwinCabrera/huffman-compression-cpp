#ifndef BITS_H
#define BITS_H

//#include <iostream>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <cctype>
#include <cstdio> //only for printf, delete after done 
#include <string>

#define BYTE_LEN 8

using std::shared_ptr;
using std::string;
using std::isdigit;

class BitSequence{
public:
    BitSequence(size_t num_bits);
    //BitSequence(uint8_t *data, size_t length);
    BitSequence() {}
    ~BitSequence();

    void set_bit(size_t bit_idx, bool set);
    void set_bits_from_num(size_t start_bit_idx, uint64_t num);
    void set_next_bit_start(size_t bit_idx);
    void set_next_bit(bool set);
    void set_next_byte_start(size_t byte_idx);
    void set_next_byte(uint8_t byte);


    bool get_bit(size_t bit_idx);
    uint8_t get_byte(size_t byte_idx);
    uint64_t get_x_bytes(uint8_t num_bytes);
    void get_next_bit_start(size_t bit_idx);
    bool get_next_bit();
    uint8_t get_next_byte();

    uint8_t* get_data() { return this->data.get();}
    size_t get_num_bits() { return this->num_bits; }
    size_t get_bytes_allocated(){ return this->bytes_allocated; }
    int get_next_bit_idx() { return this->next_bit_idx; }
    int get_next_byte_idx() { return this->next_byte_idx; }


private:
    shared_ptr<uint8_t> data;
    size_t num_bits; // this will prove to be useful when a huffman code is a sequence of zeros as it tells us the number of significant bits 
    size_t bytes_allocated;
    //uint num_significant_bits;
    int next_bit_idx;
    int next_byte_idx;
    //bool reverse;

};


string num_to_hex_str(int num);


int hex_str_to_num(string hex_str);

#endif