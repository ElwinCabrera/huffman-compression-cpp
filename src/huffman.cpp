#include "../include/huffman.h"
#include "../include/huffman_tree.h"
#include <stdlib.h>     
#include <time.h>  

#include <vector>


HuffmanCompress::HuffmanCompress(string s) {
    this->string_to_compress = s;
    
    this->num_bits_in_str = s.size() * BYTE_LEN;
}

HuffmanCompress::HuffmanCompress() {
   
}
HuffmanCompress::~HuffmanCompress() {
    

}
// uint8_t* HuffmanCompress::get_huffman_codes(){

// }

BitSequence HuffmanCompress::compress(){
    this->find_char_freq();

    for(auto& [key, value] : this->char_to_freq){
        this->htree.add_to_list(key, value);
    }
    this->htree.print_list();

    this->htree.build_huffman_tree();
    //this->htree.print_tree(this->htree.get_head());

    this->huff_codes = this->htree.generate_huffman_codes();
    this->htree.print_huff_codes();

    

    //This part is very slopy but whatever need to quickly test and will optimize later

    //I created a BitSequence class because a code can be all zeros and each zero is significant 
    //Also, I wanted to represent my huff codes all in binary to seve bandwwith when sending over network 
    //however since each data point uses variable length bit sequences and a data point can be like 
    // "0001" and another "001" it creates ambiguity on the reveiving end as to how to interpret the bit sequence
    //so unfourtunatly the huffman codes will need to be serialized as string increasing the number of bits to send
    //this is only done for the huffman codes and not the actual data as the reveiving end needs to know how to reconstruct the 
    // original tree

    // size_t huff_code_bit_len = (this->huff_codes.size() * 2) * BYTE_LEN;
    // this->num_bits_in_huff_code = huff_code_bit_len;


    // this->num_bits_in_compressed_str = 0;
    // for(char ch: this->string_to_compress){
    //     this->num_bits_in_compressed_str += this->huff_codes[ch].get_num_bits();
    // }

    // this->compressed_bitsequence = BitSequence(this->num_bits_in_huff_code + this->num_bits_in_compressed_str);

    // size_t bit_idx = 0;

    // for(auto& [ch, code] : this->huff_codes){
        
    // }

    // //size_t bit_idx = this->num_bits_in_huff_code;

    // for(char ch: this->string_to_compress){
    //     BitSequence huff_cd = this->huff_codes[ch];
    //     size_t ch_code_bit_idx = huff_cd.get_num_bits() - 1;
    //     while(ch_code_bit_idx >= 0) {
    //         this->compressed_bitsequence.set_bit(bit_idx++, huff_cd.get_bit(ch_code_bit_idx))
    //         --ch_code_bit_idx;
    //     }
    // }

    return this->compressed_bitsequence;

}


void HuffmanCompress::find_char_freq(){
    for(char c: this->string_to_compress){
        if(this->char_to_freq.find(c) != this->char_to_freq.end()){
            this->char_to_freq[c] = this->char_to_freq[c] + 1;
        } else {
            this->char_to_freq[c] = 1;
        }
    }
}

string HuffmanCompress::decompress(unordered_map<char, BitSequence> huff_codes){
    this->htree.recreate_huffman_tree(huff_codes);
    //this->htree.print_tree(this->htree.get_head());
    
    return "";
}



void HuffmanCompress::print_map(){
    for (auto& [key, value] : this->char_to_freq){
        cout << '[' << key << "] = " << value << "; ";
    }
    cout << '\n';
}
 



int main(){

    

    string s1 = "this is an example of a huffman tree";
    string s2 = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";

    string s3 = "A_DEAD_DAD_CEDED_A_BAD_BABE";

    HuffmanCompress hc(s3);
    hc.compress();

    HuffmanCompress hd;
  
    hd.decompress(hc.get_huffman_codes());


    //TODO: fix overflow issue. max huff code is 0xff (= max tree depth of 8) if we use our leading bit technique we will have an overflow in the following test.
    // string all_ascii = "";
    // uint8_t c = 0;
    // while(c < 255){
    //     all_ascii += (char) c;
    //     ++c;
    // }
    // printf("%s\n",all_ascii.c_str());

    // HuffmanCompress hc2(all_ascii);
    // hc2.compress();

    // HuffmanCompress hd2;
    
    // hd2.decompress(hc2.get_huffman_codes());
    //initialize random seed
    // srand (time(NULL));

    // //between 1 and 10
    // int random_size = rand() % 10 + 1;
    // printf("list size should be %d\n", random_size);

    // HuffmanListTree hlt;
    // for(int i = 0; i < random_size; ++i){
    //     int random_num = rand() % 32 + 1;
    //     hlt.add_to_list('a' + random_num, random_num);
    // }

    // hlt.print_list();
    // hlt.build_huffman_tree();
    // hlt.print_tree(hlt.get_head());

    // hlt.generate_huffman_codes();

    // hlt.print_huff_codes();

    


    return 0;
}