#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <cstdint>
#include <string>

using namespace std;

class HuffmanCompress {
public:
    HuffmanCompress(string s);
    uint8_t compress();
    void find_char_freq();

    string decompress();



private:
    unordered_map<char, size_t> char_to_freq;

};


#endif