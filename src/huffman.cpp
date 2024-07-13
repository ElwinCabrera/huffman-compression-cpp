#include "../include/huffman.h"
#include "../include/huffman_tree.h"
#include <stdlib.h>     
#include <time.h>  

#include <vector>



HuffmanCompress::HuffmanCompress() {
    this->init(nullptr, 0);
}

HuffmanCompress::HuffmanCompress(string s) {
    this->init(s.data(), s.size());
}

HuffmanCompress::HuffmanCompress(char *data, int data_len) {
    this->init(data, data_len);
}

HuffmanCompress::~HuffmanCompress() {
    

}

void HuffmanCompress::init(char *data, int data_len){
    this->uncompressed_data = (uint8_t*) data;
    this->uncompressed_data_len = data_len;
    this->uncompressed_data_bit_len = data_len * BYTE_LEN;

    this->huff_code_bit_len = 0;
    this->serialized_huff_code_bit_len = 0;
    this->compressed_str_bit_len = 0;
    this->compressed_total_bit_len = 0;
    
}


uint8_t* HuffmanCompress::compress(){
    this->gen_byte_freq_map();

    for(auto& [byte, freq] : this->byte_freq_map){
        this->htree.add_to_list(byte, freq);
    }
    this->htree.print_list();

    this->htree.build_huffman_tree();
    //this->htree.print_tree(this->htree.get_head());

    this->huff_codes = this->htree.generate_huffman_codes();
    this->htree.print_huff_codes();

    

    //This part is very slopy but whatever need to quickly test and will optimize later

    this->serialized_huff_codes = this->serialize_huff_codes();
    

    this->compressed_str_bit_len = 0;
    int data_idx = 0;
    while(data_idx < this->uncompressed_data_len ){
        uint8_t byte = this->uncompressed_data[data_idx++];
        this->compressed_str_bit_len += this->huff_codes[byte].get_num_bits();
    }

    uint8_t trailing_zeroes_to_rm = BYTE_LEN - (this->compressed_str_bit_len % BYTE_LEN);
    
    
    this->compressed_str_bitsequence = shared_ptr<BitSequence>(new BitSequence(this->compressed_str_bit_len));
    size_t bit_idx = 0;
    data_idx = 0;
    while(data_idx < this->uncompressed_data_len){
        uint8_t byte = this->uncompressed_data[data_idx++];
        BitSequence huff_cd = this->huff_codes[byte];
        int hcode_bit_idx = huff_cd.get_num_bits() - 1;
        while(hcode_bit_idx >= 0) {
            bool set = huff_cd.get_bit(hcode_bit_idx);
            this->compressed_str_bitsequence->set_bit(bit_idx, set);
            --hcode_bit_idx;
            ++bit_idx;
        }
    }

    size_t total_bytes = this->serialized_huff_code_byte_len + this->compressed_str_bitsequence->get_bytes_allocated() + 1;
    this->compressed_total_bit_len = total_bytes * BYTE_LEN;

    this->compressed_data = (uint8_t*) malloc(total_bytes);

    uint8_t *tmp_ptr = this->compressed_data;
    memcpy(tmp_ptr, this->serialized_huff_codes.get(), this->serialized_huff_code_byte_len);
    tmp_ptr += this->serialized_huff_code_byte_len;
    memcpy(tmp_ptr, this->compressed_str_bitsequence->get_data(), this->compressed_str_bitsequence->get_bytes_allocated());
    tmp_ptr += this->compressed_str_bitsequence->get_bytes_allocated();
    *tmp_ptr = trailing_zeroes_to_rm;

    return this->compressed_data;

}




uint8_t* HuffmanCompress::decompress(uint8_t *data, int len){
    this->compressed_total_byte_len = len;
    this->compressed_total_bit_len = len * BYTE_LEN;
    
    
    this->huff_codes = this->deserialize_huff_codes(data);
    
    this->htree.recreate_huffman_tree(this->huff_codes);
    this->htree.print_huff_codes();

    this->compressed_str_bitsequence = this->create_bit_seq(data + this->serialized_huff_code_byte_len, len - this->serialized_huff_code_byte_len);

    char *decoded_data = (char*) this->htree.decode_bit_seq(this->compressed_str_bitsequence);
    this->uncompressed_data = (uint8_t*) decoded_data;

    printf("Decoded data: %s\n", decoded_data);
    
    string res(decoded_data);
    this->uncompressed_data_bit_len = res.size() * BYTE_LEN;

    this->print_stats();

    return this->uncompressed_data;
}




//instead of manually finding the size and manually allocating space we could also use a string object that will dynamically 
//allocate more space for us but we are hard core so we do things the hard way :P 
//(initially this was the method i used but was testing things and now this works and lazy to fixit untill its time to do code cleanup which will be when im done, which is always never :P)
shared_ptr<uint8_t> HuffmanCompress::serialize_huff_codes(){
    //unfourunatly will have to serialize as string
    //<char>_<bit_len>_<code-in-hex>_<char>...
    //1 byte   1 byte      X bytes

    //Note: we use a bit lenth of 1 byte. if theres ever a case where we have a tree with depth > 255 this will fail causing incorrect data when decompressing most systems are 32 or 64 bits so a tree depth above that seems like unlikely so i think we are safe with one byte here.

    string res = "";

    unordered_map<char, string> m;
    size_t total_serialized_huff_code_byte_len = (this->huff_codes.size() * 2) + 1;
    for(auto& [ch, code]: this->huff_codes){
        uint64_t num = code.get_x_bytes(8);
        string num_to_str = "_" + num_to_hex_str(num) + "_";
        m[ch] = num_to_str;

        total_serialized_huff_code_byte_len += num_to_str.size();
        // res += ch;
        // res += (char) code.get_num_bits(); 
        // uint64_t num = code.get_x_bytes(8);
        // res += "_" + num_to_hex_str(num) + "_";
    }
    
    uint8_t *data = (uint8_t*) malloc(total_serialized_huff_code_byte_len);
    memset(data, '\0', total_serialized_huff_code_byte_len);

    uint8_t *data_ptr = data;
    for(auto& [ch, str]: m){
        *data_ptr++ = ch;
        *data_ptr++ = (uint8_t) this->huff_codes[ch].get_num_bits();
        for(char c: str){
            *data_ptr++ = c;
        }
    }
    this->serialized_huff_code_byte_len =  total_serialized_huff_code_byte_len;
    this->serialized_huff_code_bit_len = total_serialized_huff_code_byte_len * BYTE_LEN;
    
    //string s((char*)data);

    
    return shared_ptr<uint8_t>(data);
}

unordered_map<char, BitSequence> HuffmanCompress::deserialize_huff_codes(uint8_t *data){
    unordered_map<char, BitSequence> res;
    int huff_code_len = 0;
    int bit_idx = 0;
    while(*data != '\0') {
        char ch = *data++;
        uint8_t seq_len = *data++;
        BitSequence bit_seq(seq_len);
        if(*data++ == '_'){
            string hex_str = "";
            while(*data != '_'){
                hex_str += *data++;
            }
            huff_code_len += hex_str.size() + 2;
            if(*data == '_' ) data++;
            size_t num = hex_str_to_num(hex_str);
            bit_seq.set_bits_from_num(0, num);
            
        }
        res[ch] = bit_seq; 
        //huff_code_len -= 2;
        huff_code_len += 2;
    }
    this->serialized_huff_code_bit_len = (huff_code_len + 1) * BYTE_LEN;
    this->serialized_huff_code_byte_len = this->serialized_huff_code_bit_len / BYTE_LEN;
    if(this->serialized_huff_code_bit_len % BYTE_LEN) this->serialized_huff_code_byte_len += 1;
    
    // need to create a temp pointer that points to data before doing this. Also, this is nice to have but not nessasary at the moment
    // this->serialized_huff_codes = (uint8_t*) malloc(this->serialized_huff_code_byte_len );
    // data++;
    // memcpy(this->serialized_huff_codes, data, this->serialized_huff_code_byte_len);

    return res;

}


shared_ptr<BitSequence> HuffmanCompress::create_bit_seq(uint8_t *data, int length){
    uint8_t trailing_zeroes_to_rm = data[length - 1];
    --length;

    this->compressed_str_bit_len = (length * BYTE_LEN) - trailing_zeroes_to_rm;
    shared_ptr<BitSequence> bit_seq(new BitSequence(this->compressed_str_bit_len));
    

    bit_seq->set_next_byte_start(0);
    int byte_idx = 0;
    while(byte_idx < length){
        bit_seq->set_next_byte(*data++);
        ++byte_idx;
    }
    return bit_seq;
}

void HuffmanCompress::gen_byte_freq_map(){
    int data_idx = 0;
    while(data_idx < this->uncompressed_data_len){
        uint8_t byte = this->uncompressed_data[data_idx++];
        if(this->byte_freq_map.find(byte) != this->byte_freq_map.end()){
            this->byte_freq_map[byte] = this->byte_freq_map[byte] + 1;
        } else {
            this->byte_freq_map[byte] = 1;
        }
    }
}


void HuffmanCompress::print_byte_freq_map(){
    for (auto& [byte, freq] : this->byte_freq_map){
        cout << '[' << byte << "] = " << freq << "; ";
    }
    cout << '\n';
}

void HuffmanCompress::print_stats(){
    size_t compressed_byte_len = this->compressed_total_bit_len / BYTE_LEN;
    size_t compressed_data_byte_len = this->compressed_str_bit_len / BYTE_LEN;

    if(this->compressed_total_bit_len % BYTE_LEN != 0) compressed_byte_len += 1;
    if(this->compressed_str_bit_len % BYTE_LEN != 0) compressed_data_byte_len += 1;

    printf("\nUncompressed: %d(=%d bytes)\n", this->uncompressed_data_bit_len, this->uncompressed_data_bit_len/ BYTE_LEN);
    printf("Compressed: %d(=%d bytes)\n", this->compressed_total_bit_len, compressed_byte_len);
    printf("\tSerialized Huff Code: %d(=%d bytes)\n",this->serialized_huff_code_bit_len, this->serialized_huff_code_bit_len/BYTE_LEN);
    printf("\tData: %d(=%d bytes)\n", this->compressed_str_bit_len, compressed_data_byte_len);

    int diff_bits = this->uncompressed_data_bit_len - this->compressed_total_bit_len;
    int diff_bytes = diff_bits / BYTE_LEN;
    if(diff_bits % BYTE_LEN != 0) diff_bytes += 1;

    printf("Saved/Gained by compressing: %d(=%d bytes)\n",diff_bits, diff_bytes);
    
   
}
