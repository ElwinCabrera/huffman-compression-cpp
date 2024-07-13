#include "include/huffman.h"


int main(){

    // string s1 = "this is an example of a huffman tree";
    // string s2 = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";

    // string s3 = "A_DEAD_DAD_CEDED_A_BAD_BABE";

    // string really_long = "";

    // HuffmanCompress hc(really_long);
    // //HuffmanCompress hc(s3);
    // uint8_t *compressed_data = hc.compress();
    // hc.print_stats();

    // HuffmanCompress hd;
  
    // uint8_t *uncompressed_data = hd.decompress(compressed_data, hc.get_compressed_len());

    // free(compressed_data);
    // free(uncompressed_data);

    // compressed_data = nullptr;
    // uncompressed_data = nullptr;


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