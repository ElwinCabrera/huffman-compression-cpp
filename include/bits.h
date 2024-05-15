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
    //BitSequence(uint8_t *data, size_t length);
    BitSequence() {}
    ~BitSequence();

    void set_bit(size_t bit_idx, bool set);
    bool get_bit(size_t bit_idx);
    uint8_t get_byte(size_t byte_idx);
    uint64_t get_x_bytes(uint8_t num_bytes);

    uint8_t* get_data() { return this->data.get();}
    size_t get_num_bits() { return this->num_bits; }
    size_t get_bytes_allocated(){ return this->bytes_allocated; }

private:
    shared_ptr<uint8_t> data;
    size_t num_bits; // this will prove to be useful when a huffman code is a sequence of zeros as it tells us the number of significant bits 
    size_t bytes_allocated;
    //uint num_significant_bits;

};

#endif