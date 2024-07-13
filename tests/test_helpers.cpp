#include "test_globals.h"

#include <time.h>  

string get_all_ascii_chars(){
    string all_ascii = "";
    uint8_t c = 1;
    while(c < 255){
        all_ascii += (char) c;
        ++c;
    }
    return all_ascii;
}

uint generate_random_num(int min, int max){
    srand(time(NULL));
    return (rand() % max) + min;
}


unordered_map<uint8_t, uint> gen_byte_freq_map(uint8_t *data, int len){
    int idx = 0;
    unordered_map<uint8_t, uint> byte_freq;
    while(idx < len){
        uint8_t byte = data[idx++];
        if(byte_freq.find(byte) != byte_freq.end()){
            byte_freq[byte] = byte_freq[byte] + 1;
        } else {
            byte_freq[byte] = 1;
        }
    }
    return byte_freq;
}