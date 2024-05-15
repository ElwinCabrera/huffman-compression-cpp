#include "../include/bits.h"

BitSequence::BitSequence(size_t num_bits){
    this->num_bits = num_bits;
    this->bytes_allocated = num_bits / BYTE_LEN;
    if(num_bits % BYTE_LEN != 0) this->bytes_allocated += 1;
    this->data = shared_ptr<uint8_t>((uint8_t*) malloc(this->bytes_allocated));
    memset(this->data.get(), '\0', this->bytes_allocated);
}
BitSequence::~BitSequence() {
    
}

void BitSequence::set_bit(size_t bit_idx, bool set) {
    if(bit_idx < 0 || bit_idx >= this->num_bits){
        return;
    }
    size_t byte_idx = bit_idx / BYTE_LEN;
    size_t bit_idx_in_byte = bit_idx % BYTE_LEN;

    if(set){
        (this->data.get())[byte_idx] |= (1 << bit_idx_in_byte);
    } else {
        (this->data.get())[byte_idx] &= ~(1 << bit_idx_in_byte);
    }
}

bool BitSequence::get_bit(size_t bit_idx) {
    if(bit_idx < 0 || bit_idx >= this->num_bits){
        printf("get_bit idx out of bounds\n");
        return false;
    }
    size_t byte_idx = bit_idx / BYTE_LEN;
    size_t bit_idx_in_byte = bit_idx % BYTE_LEN;

    return ((this->data.get())[byte_idx] >> bit_idx_in_byte) & 1;

}

uint8_t BitSequence::get_byte(size_t byte_idx) {
    return (this->data.get())[byte_idx];
}