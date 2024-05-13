#include "../include/huffman.h"
#include "../include/huffman_tree.h"
#include <stdlib.h>     
#include <time.h>  

#include <vector>

int main(){

    

    //initialize random seed
    srand (time(NULL));

    //between 1 and 10
    int random_size = rand() % 10 + 1;
    printf("list size should be %d\n", random_size);

    HuffmanListTree hlt;
    for(int i = 0; i < random_size; ++i){
        int random_num = rand() % 32 + 1;
        hlt.add_to_list('a' + random_num, random_num);
    }

    hlt.print();


    return 0;
}