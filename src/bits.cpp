#include "../include/bits.h"

BitSequence::BitSequence(size_t num_bits): next_bit_idx(-1){
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
        printf("set_bit idx out of bounds\n");
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

void BitSequence::set_bits_from_num(size_t start_bit_idx, uint64_t num){
    if(start_bit_idx < 0 || start_bit_idx >= this->num_bits){
        printf("set_bits_from_num idx out of bounds\n");
        return;
    }
    while(num && start_bit_idx < this->num_bits){
        this->set_bit(start_bit_idx++, num & 0x1);
        num >>= 1;
    }
}


void BitSequence::set_next_bit_start(size_t bit_idx){
    if(bit_idx < 0 || bit_idx >= this->num_bits){
        printf("set_next_bit_start idx out of bounds\n");
        return;
    }
    this->next_bit_idx = bit_idx;

}
void BitSequence::set_next_bit(bool set){
    if(this->next_bit_idx == -1){
        printf("error set_next_bit never called\n");
        return;
    }
    return this->set_bit((this->next_bit_idx)++, set);
}


void BitSequence::set_next_byte_start(size_t byte_idx){
    if(byte_idx < 0 || byte_idx >= this->bytes_allocated){
        printf("set_next_byte_start idx out of bounds\n");
        return;
    }
    this->next_byte_idx = byte_idx;

}
void BitSequence::set_next_byte(uint8_t byte){
    if(this->next_byte_idx == -1){
        printf("error set_next_byte never called\n");
        return;
    }
    (this->data.get())[this->next_byte_idx++] = byte;
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
    if(byte_idx < 0 || byte_idx >= this->bytes_allocated){
        printf("get_byte idx out of bounds\n");
        return false;
    }
    return (this->data.get())[byte_idx];
}

uint64_t BitSequence::get_x_bytes(uint8_t num_bytes){
    uint64_t res = 0;
    if(num_bytes > this->bytes_allocated) num_bytes = this->bytes_allocated;
    uint8_t byte_idx = 0;
    while(num_bytes){
        res <<= BYTE_LEN;
        res |= this->get_byte(num_bytes - 1);
        
        --num_bytes;
    }
    return res;
}

void BitSequence::get_next_bit_start(size_t bit_idx){
    if(bit_idx < 0 || bit_idx >= this->num_bits){
        bit_idx = this->num_bits -1;
        printf("get_next_bit_start idx out of bounds\n");
        //return;
    }
    this->next_bit_idx = bit_idx;
}

bool BitSequence::get_next_bit() {
    if(this->next_bit_idx == -1){
        printf("error get_next_start never called\n");
        return false;
    }
    return this->get_bit((this->next_bit_idx)++);
}









string num_to_hex_str(int num){
    string hex = "";
    while(num > 0){
        int remainder = num % 16;
        char c = '0' + remainder;
        if(remainder >= 10) {
            remainder -= 10;
            c = 'A' + remainder;
        }
        num /= 16;
        hex = c + hex;
    }

    return hex;
}


int hex_str_to_num(string hex_str){
    int num = 0;
    int idx = hex_str.size() - 1;
    while(idx >= 0){
        char c = hex_str.at(idx);
        if(isdigit(c)) num += (c - '0') * pow(16, idx);
        else num += ((c - 'A') + 10) * pow(16, idx);
        --idx;
    }
    return num;
  
}