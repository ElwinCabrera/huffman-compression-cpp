#include "../include/huffman.h"
#include "../include/huffman_tree.h"
#include <stdlib.h>     
#include <time.h>  

#include <vector>


HuffmanCompress::HuffmanCompress(string s) {
    this->uncompressed_str = s;
    this->uncompressed_str_bit_len = s.size() * BYTE_LEN;

    this->huff_code_bit_len = 0;
    this->serialized_huff_code_bit_len = 0;
    this->compressed_str_bit_len = 0;
    this->compressed_total_bit_len = 0;
}

HuffmanCompress::HuffmanCompress() {
   
}
HuffmanCompress::~HuffmanCompress() {
    

}


uint8_t* HuffmanCompress::compress(){
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

    this->serialized_huff_codes = this->serialize_huff_codes();
    

    this->compressed_str_bit_len = 0;
    for(char ch: this->uncompressed_str){
        this->compressed_str_bit_len += this->huff_codes[ch].get_num_bits();
    }

    

    this->compressed_str_bitsequence = BitSequence(this->compressed_str_bit_len);
    size_t bit_idx = 0;
    for(char ch: this->uncompressed_str){
        BitSequence huff_cd = this->huff_codes[ch];
        int hcode_bit_idx = huff_cd.get_num_bits() - 1;
        while(hcode_bit_idx >= 0) {
            bool set = huff_cd.get_bit(hcode_bit_idx);
            this->compressed_str_bitsequence.set_bit(bit_idx, set);
            --hcode_bit_idx;
            ++bit_idx;
        }
    }

    size_t total_bytes = this->serialized_huff_code_byte_len + this->compressed_str_bitsequence.get_bytes_allocated();
    this->compressed_total_bit_len = total_bytes * BYTE_LEN;

    this->compressed_data = (uint8_t*) malloc(total_bytes );

    uint8_t *tmp_ptr = this->compressed_data;
    memcpy(tmp_ptr, this->serialized_huff_codes, this->serialized_huff_code_byte_len);
    tmp_ptr += this->serialized_huff_code_byte_len;
    memcpy(tmp_ptr, this->compressed_str_bitsequence.get_data(), this->compressed_str_bitsequence.get_bytes_allocated());

    return this->compressed_data;

}


void HuffmanCompress::find_char_freq(){
    for(char c: this->uncompressed_str){
        if(this->char_to_freq.find(c) != this->char_to_freq.end()){
            this->char_to_freq[c] = this->char_to_freq[c] + 1;
        } else {
            this->char_to_freq[c] = 1;
        }
    }
}


string HuffmanCompress::decompress(uint8_t *data, int len){
    
    this->huff_codes = this->deserialize_huff_codes(data);
    this->htree.recreate_huffman_tree(this->huff_codes);
    this->htree.print_huff_codes();

    int huff_codes_len_with_header = (this->serialized_huff_code_byte_len + 1);
    this->compressed_str_bitsequence = this->create_bit_seq(data + huff_codes_len_with_header, len - huff_codes_len_with_header);

    char *decoded_data = (char*) this->htree.decode_bit_seq(this->compressed_str_bitsequence);

    printf("Decoded data: %s\n", decoded_data);

    string res(decoded_data);
    return res;
}




//instead of manually finding the size and manually allocating space we could also use a string object that will dynamically 
//allocate more space for us but we are hard core so we do things the hard way lol :P 
//(initially this was the method i used but was testing things and now this works and lazy to fixit untill its time to do code cleanup which will be when im done, which is always never lol :P)
uint8_t* HuffmanCompress::serialize_huff_codes(){
    //unfourunatly will have to serialize as string
    //<char>_<bit_len>_<code-in-hex>_<char>...
    //1 byte   1 byte      X bytes

    //Note: we use a bit lenth of i byte. if theres ever a case where we have a tree with depth > 255 this will fail causing incorrect data when decompressing most systems are 32 or 64 bits so a tree depth above that seems like unlikely so i think we are safe with one byte here.

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
    memset(data, '\0', total_serialized_huff_code_byte_len - 1);

    uint8_t *data_ptr = data;
    *data_ptr++ = (uint8_t) total_serialized_huff_code_byte_len - 1;
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

    
    return data;
}

unordered_map<char, BitSequence> HuffmanCompress::deserialize_huff_codes(uint8_t *data){
    unordered_map<char, BitSequence> res;
    int huff_code_len = *data++;
    this->huff_code_bit_len = huff_code_len * BYTE_LEN;
    int bit_idx = 0;
    while(huff_code_len > 0) {
        char ch = *data++;
        uint8_t seq_len = *data++;
        BitSequence bit_seq(seq_len);
        if(*data++ == '_'){
            string hex_str = "";
            while(*data != '_'){
                hex_str += *data++;
            }
            huff_code_len -= hex_str.size() + 2;
            if(*data == '_' && huff_code_len - 2 > 0) data++;
            uint8_t num = hex_str_to_num(hex_str);
            bit_seq.set_bits_from_num(0, num);
            
        }
        res[ch] = bit_seq; 
        huff_code_len -= 2;
    }

    this->serialized_huff_code_byte_len = this->huff_code_bit_len / BYTE_LEN;
    if(this->huff_code_bit_len % BYTE_LEN) this->serialized_huff_code_byte_len += 1;
    
    // need to create a temp pointer that points to data before doing this. Also, this is nice to have but not nessasary at the moment
    // this->serialized_huff_codes = (uint8_t*) malloc(this->serialized_huff_code_byte_len );
    // data++;
    // memcpy(this->serialized_huff_codes, data, this->serialized_huff_code_byte_len);

    return res;

}


BitSequence HuffmanCompress::create_bit_seq(uint8_t *data, int length){
    
    BitSequence bit_seq(length * BYTE_LEN);
    bit_seq.set_next_byte_start(0);
    int byte_idx = 0;
    while(byte_idx < length){
        bit_seq.set_next_byte(*data++);
        ++byte_idx;
    }
    
    return bit_seq;
}

void HuffmanCompress::print_map(){
    for (auto& [key, value] : this->char_to_freq){
        cout << '[' << key << "] = " << value << "; ";
    }
    cout << '\n';
}

void HuffmanCompress::print_stats(){
    size_t compressed_byte_len = this->compressed_total_bit_len / BYTE_LEN;
    size_t compressed_data_byte_len = this->compressed_str_bit_len / BYTE_LEN;

    if(this->compressed_total_bit_len % BYTE_LEN != 0) compressed_byte_len += 1;
    if(this->compressed_str_bit_len % BYTE_LEN != 0) compressed_data_byte_len += 1;

    printf("\nUncompressed: %d(=%d bytes)\n", this->uncompressed_str_bit_len, this->uncompressed_str_bit_len/ BYTE_LEN);
    printf("Compressed: %d(=%d bytes)\n", this->compressed_total_bit_len, compressed_byte_len);
    printf("\tSerialized Huff Code: %d(=%d bytes)\n",this->serialized_huff_code_bit_len, this->serialized_huff_code_bit_len/BYTE_LEN);
    printf("\tData: %d(=%d bytes)\n", this->compressed_str_bit_len, compressed_data_byte_len);

    int diff_bits = this->uncompressed_str_bit_len - this->compressed_total_bit_len;
    int diff_bytes = diff_bits / BYTE_LEN;
    if(diff_bits % BYTE_LEN != 0) diff_bytes += 1;

    printf("Saved/Gained by compressing: %d(=%d bytes)\n",diff_bits, diff_bytes);
    
   
}



int main(){

    string s1 = "this is an example of a huffman tree";
    string s2 = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";

    string s3 = "A_DEAD_DAD_CEDED_A_BAD_BABE";

    string really_long = "In computer science and information theory, a Huffman code is a particular type of optimal prefix code that is commonly used for lossless data compression. The process of finding or using such a code is Huffman coding, an algorithm developed by David A. Huffman while he was a Sc.D. student at MIT, and published in the 1952 paper A Method for the Construction of Minimum-Redundancy Codes.[1] The output from Huffman's algorithm can be viewed as a variable-length code table for encoding a source symbol (such as a character in a file). The algorithm derives this table from the estimated probability or frequency of occurrence (weight) for each possible value of the source symbol. As in other entropy encoding methods, more common symbols are generally represented using fewer bits than less common symbols. Huffman's method can be efficiently implemented, finding a code in time linear to the number of input weights if these weights are sorted.[2] However, although optimal among methods encoding symbols separately, Huffman coding is not always optimal among all compression methods - it is replaced with arithmetic coding[3] or asymmetric numeral systems[4] if a better compression ratio is required. History In 1951, David A. Huffman and his MIT information theory classmates were given the choice of a term paper or a final exam. The professor, Robert M. Fano, assigned a term paper on the problem of finding the most efficient binary code. Huffman, unable to prove any codes were the most efficient, was about to give up and start studying for the final when he hit upon the idea of using a frequency-sorted binary tree and quickly proved this method the most efficient.[5] In doing so, Huffman outdid Fano, who had worked with Claude Shannon to develop a similar code. Building the tree from the bottom up guaranteed optimality, unlike the top-down approach of Shannon–Fano coding. Terminology Huffman coding uses a specific method for choosing the representation for each symbol, resulting in a prefix code (sometimes called prefix-free codes, that is, the bit string representing some particular symbol is never a prefix of the bit string representing any other symbol). Huffman coding is such a widespread method for creating prefix codes that the term Huffman code is widely used as a synonym for prefix code even when such a code is not produced by Huffman's algorithm. Compression Visualisation of the use of Huffman coding to encode the message A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED. In steps 2 to 6, the letters are sorted by increasing frequency, and the least frequent two at each step are combined and reinserted into the list, and a partial tree is constructed. The final tree in step 6 is traversed to generate the dictionary in step 7. Step 8 uses it to encode the message. A source generates 4 different symbols { a 1 , a 2 , a 3 , a 4 } {a_{1},a_{2},a_{3},a_{4}}} with probability { 0.4 ; 0.35 ; 0.2 ; 0.05 } {0.4;0.35;0.2;0.05}. A binary tree is generated from left to right taking the two least probable symbols and putting them together to form another equivalent symbol having a probability that equals the sum of the two symbols. The process is repeated until there is just one symbol. The tree can then be read backwards, from right to left, assigning different bits to different branches. The final Huffman code is: Symbol 	Code a1 	0 a2 	10 a3 	110 a4 	111 The standard way to represent a signal made of 4 symbols is by using 2 bits/symbol, but the entropy of the source is 1.74 bits/symbol. If this Huffman code is used to represent the signal, then the average length is lowered to 1.85 bits/symbol; it is still far from the theoretical limit because the probabilities of the symbols are different from negative powers of two. The technique works by creating a binary tree of nodes. These can be stored in a regular array, the size of which depends on the number of symbols, n . A node can be either a leaf node or an internal node. Initially, all nodes are leaf nodes, which contain the symbol itself, the weight (frequency of appearance) of the symbol and optionally, a link to a parent node which makes it easy to read the code (in reverse) starting from a leaf node. Internal nodes contain a weight, links to two child nodes and an optional link to a parent node. As a common convention, bit '0' represents following the left child and bit '1' represents following the right child. A finished tree has up to n  leaf nodes and n − 1  internal nodes. A Huffman tree that omits unused symbols produces the most optimal code lengths. The process begins with the leaf nodes containing the probabilities of the symbol they represent. Then, the process takes the two nodes with smallest probability, and creates a new internal node having these two nodes as children. The weight of the new node is set to the sum of the weight of the children. We then apply the process again, on the new internal node and on the remaining nodes (i.e., we exclude the two leaf nodes), we repeat this process until only one node remains, which is the root of the Huffman tree. The simplest construction algorithm uses a priority queue where the node with lowest probability is given highest priority: Create a leaf node for each symbol and add it to the priority queue. While there is more than one node in the queue: Remove the two nodes of highest priority (lowest probability) from the queue Create a new internal node with these two nodes as children and with probability equal to the sum of the two nodes' probabilities. Add the new node to the queue. The remaining node is the root node and the tree is complete. Since efficient priority queue data structures require O(log n) time per insertion, and a tree with n leaves has 2n−1 nodes, this algorithm operates in O(n log n) time, where n is the number of symbols. If the symbols are sorted by probability, there is a linear-time (O(n)) method to create a Huffman tree using two queues, the first one containing the initial weights (along with pointers to the associated leaves), and combined weights (along with pointers to the trees) being put in the back of the second queue. This assures that the lowest weight is always kept at the front of one of the two queues: Start with as many leaves as there are symbols. Enqueue all leaf nodes into the first queue (by probability in increasing order so that the least likely item is in the head of the queue). While there is more than one node in the queues: Dequeue the two nodes with the lowest weight by examining the fronts of both queues. Create a new internal node, with the two just-removed nodes as children (either node can be either child) and the sum of their weights as the new weight. Enqueue the new node into the rear of the second queue. The remaining node is the root node; the tree has now been generated. Once the Huffman tree has been generated, it is traversed to generate a dictionary which maps the symbols to binary codes as follows: Start with current node set to the root. If node is not a leaf node, label the edge to the left child as 0 and the edge to the right child as 1. Repeat the process at both the left child and the right child. The final encoding of any symbol is then read by a concatenation of the labels on the edges along the path from the root node to the symbol. In many cases, time complexity is not very important in the choice of algorithm here, since n here is the number of symbols in the alphabet, which is typically a very small number (compared to the length of the message to be encoded); whereas complexity analysis concerns the behavior when n grows to be very large. It is generally beneficial to minimize the variance of codeword length. For example, a communication buffer receiving Huffman-encoded data may need to be larger to deal with especially long symbols if the tree is especially unbalanced. To minimize variance, simply break ties between queues by choosing the item in the first queue. This modification will retain the mathematical optimality of the Huffman coding while both minimizing variance and minimizing the length of the longest character code. Decompression Generally speaking, the process of decompression is simply a matter of translating the stream of prefix codes to individual byte values, usually by traversing the Huffman tree node by node as each bit is read from the input stream (reaching a leaf node necessarily terminates the search for that particular byte value). Before this can take place, however, the Huffman tree must be somehow reconstructed. In the simplest case, where character frequencies are fairly predictable, the tree can be preconstructed (and even statistically adjusted on each compression cycle) and thus reused every time, at the expense of at least some measure of compression efficiency. Otherwise, the information to reconstruct the tree must be sent a priori. A naive approach might be to prepend the frequency count of each character to the compression stream. Unfortunately, the overhead in such a case could amount to several kilobytes, so this method has little practical use. If the data is compressed using canonical encoding, the compression model can be precisely reconstructed with just B ⋅ 2 B * 2^B bits of information (where B is the number of bits per symbol). Another method is to simply prepend the Huffman tree, bit by bit, to the output stream. For example, assuming that the value of 0 represents a parent node and 1 a leaf node, whenever the latter is encountered the tree building routine simply reads the next 8 bits to determine the character value of that particular leaf. The process continues recursively until the last leaf node is reached; at that point, the Huffman tree will thus be faithfully reconstructed. The overhead using such a method ranges from roughly 2 to 320 bytes (assuming an 8-bit alphabet). Many other techniques are possible as well. In any case, since the compressed data can include unused trailing bits the decompressor must be able to determine when to stop producing output. This can be accomplished by either transmitting the length of the decompressed data along with the compression model or by defining a special code symbol to signify the end of input (the latter method can adversely affect code length optimality, however). ";


    HuffmanCompress hc(s3);
    uint8_t *data = hc.compress();
    hc.print_stats();

    HuffmanCompress hd;
  
    hd.decompress(data, hc.get_compressed_len());


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