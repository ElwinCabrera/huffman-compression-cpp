#ifndef BITS_H
#define BITS_H

//#include <iostream>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <cstdio> //only for printf, delete after done 

#define BYTE_LEN 8

using std::shared_ptr;

class BitSequence{
public:
    BitSequence(size_t num_bits);
    BitSequence() {}
    ~BitSequence();

    void set_bit(size_t bit_idx, bool set);
    bool get_bit(size_t bit_idx);
    uint8_t get_byte(size_t byte_idx);

    shared_ptr<uint8_t> get_data() { return this->data;}
    size_t get_num_bits() { return this->num_bits; }

private:
    shared_ptr<uint8_t> data;
    size_t num_bits; // this will prove to be useful when a huffman code is a sequence of zeros as it tells us the number of significant bits 
    size_t bytes_allocated;
    //uint num_significant_bits;

};

#endif