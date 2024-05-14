#include "../include/huffman.h"
#include "../include/huffman_tree.h"
#include <stdlib.h>     
#include <time.h>  

#include <vector>


HuffmanCompress::HuffmanCompress(string s) {
    this->string_to_compress = s;

}

HuffmanCompress::HuffmanCompress() {
    

}

// uint8_t* HuffmanCompress::get_huffman_codes(){

// }

uint8_t HuffmanCompress::compress(){
    this->find_char_freq();
    for(auto& [key, value] : this->char_to_freq){
        this->htree.add_to_list(key, value);
    }
    this->htree.print_list();

    this->htree.build_huffman_tree();
    this->htree.print_tree(this->htree.get_head());

    this->huff_codes = this->htree.generate_huffman_codes();
    this->htree.print_huff_codes();

    return 0;

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

string HuffmanCompress::decompress(vector<tuple<char, uint8_t>> huff_codes, uint8_t *data){
    this->htree.recreate_huffman_tree(huff_codes);
    this->htree.print_tree(this->htree.get_head());
    
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
    uint8_t d[] = {0};
    hd.decompress(hc.get_huffman_codes(), d);
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