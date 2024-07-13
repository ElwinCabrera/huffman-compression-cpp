#ifndef TEST_GLOBALS_H
#define TEST_GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <map>
using std::string;
using std:map;


//Test helpers
string get_all_ascii_chars();
uint generate_random_num(int min, int max);
unordered_map<uint8_t, uint> gen_byte_freq_map(uint8_t *data, int len);

#endif
