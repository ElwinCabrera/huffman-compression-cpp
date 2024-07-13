#include "test_globals.h"
#include "../include/huffman_tree.h"
#include "../include/bits.h"

//#include <map>
#include <unordered_map>
//using std::map;
using std::unordered_map;


bool verify_list(HuffmanListTree *hlt, unordered_map<uint8_t, uint> byte_freq_map){
    int len = 0;
    Node *curr = hlt.get_head();
    unordered_map<uint8_t, int> byte_to_num_times_seen;
    
    while(curr != nullptr){
        uint8_t byte = (uint8_t) curr->data;
        
        if(byte_freq_map.find(byte) == byte_freq_map.end()) return false;
        int expected_weight = byte_freq_map[byte];

        if(curr->weight != expected_weight) return false;


        if(byte_to_num_times_seen.find(byte) != byte_to_num_times_seen.end()) byte_to_num_times_seen[byte] = byte_to_num_times_seen[byte] + 1;
        else byte_to_num_times_seen[byte] = 1;

        curr = curr->next_in_list;
        ++len;
    }
    
    if(len != byte_freq_map.size()) return false;

    for(auto& [byte, num_seen]: byte_to_num_times_seen){
        if(num_seen > 1) return false;
    }

    


    return true;
}

bool verify_tree(HuffmanListTree *hlt, uint8_t *expected){

    return false;

}




void test_hlt_random_data() {

    
    int ran_data_size = generate_random_num(1, 10); //pseudo random but whatever
    printf("list size should be %d\n", ran_data_size);

    
    uint8_t *data = (uint8_t*) malloc(ran_data_size);
    memset(data, '\0', ran_data_size);
    uint8_t *data_ptr = data;
    for(int i = 0; i < ran_data_size; ++i){
        int ran_char = 'a' + generate_random_num(1, 32);
        *data_ptr++ += ran_char;
        //int random_weight = generate_random_num(1, 10);
        //hlt.add_to_list(random_char, random_weight);
    }

    

    unordered_map<uint8_t, uint> byte_freq_map = gen_byte_freq_map(data, ran_data_size);

    HuffmanListTree hlt;

    for(auto& [byte, freq] : byte_freq_map){
        hlt.add_to_list(byte, freq);
    }
    // hlt.print_list();

    // hlt.build_huffman_tree();
    // //hlt.print_tree(hlt.get_head());

    // huff_codes = hlt.generate_huffman_codes();
    // //hlt.print_huff_codes();
}


bool huffman_list_tree_test_all(){
    
}