#include "../include/huffman_tree.h"

Node::Node(char data, int weight, bool ignore_data): data(data), weight(weight), next_in_list(nullptr), prev_in_list(nullptr), left_tree(nullptr), right_tree(nullptr), ignore_data(ignore_data){
}



HuffmanListTree::HuffmanListTree(): head(nullptr), last(nullptr){

}

HuffmanListTree::~HuffmanListTree(){
    if(!(this->head)) return;
    if(this->head->left_tree == nullptr && this->head->right_tree == nullptr){
        this->free_list();
    } else {
        this->free_tree(this->head);
    }
    // if(this->head){
    //     free(this->head);
    //     this->head = nullptr;
    // } 
}

void  HuffmanListTree::add_to_list(char c, int weight){
    Node *new_node = new Node(c, weight);
    this->insert_node_in_list(new Node(c, weight));
}

void  HuffmanListTree::sort_list(){

    //This is a very inefficient way of sorting but then again this function is never used 
    // because sorting happens at new node insertion time
    Node *ptr1 = this->head;
    while(ptr1 != nullptr){
        Node *ptr2 = ptr1->next_in_list;
        while(ptr2 != nullptr){
            if(ptr2->weight < ptr1->weight){
                //swap nodes
                Node *ptr1_hold_next = ptr1->next_in_list;
                Node *ptr1_hold_prev = ptr1->prev_in_list;

                ptr1->next_in_list = ptr2->next_in_list;
                ptr1->prev_in_list = ptr2->prev_in_list;

                if(ptr2->next_in_list != nullptr) ptr2->next_in_list->prev_in_list = ptr1;
                if(ptr2->prev_in_list != nullptr) ptr2->prev_in_list->next_in_list = ptr1;
                if(ptr1_hold_next != nullptr) ptr1_hold_next->prev_in_list = ptr2;
                if(ptr1_hold_prev != nullptr) ptr1_hold_prev->next_in_list = ptr2;


                if(ptr1 == this->head) this->head = ptr2;
                ptr2 = ptr1;
            }
            ptr2 = ptr2->next_in_list;
        }
        ptr1 = ptr1->next_in_list;
    }
    

}

Node* HuffmanListTree::build_huffman_tree(){

    Node *curr = this->head;
    while(curr != nullptr && curr->next_in_list != nullptr){
        Node *left = curr;
        Node *right = curr->next_in_list;
        Node *next = curr->next_in_list->next_in_list;
        this->detach_node_from_list(left);
        this->detach_node_from_list(right);


        Node *new_node = new Node('\0', left->weight + right->weight);
        new_node->ignore_data = true;
        new_node->left_tree = left;
        new_node->right_tree = right;

        //this->head = next;
        
        // if(next == nullptr) {
        //     //this->head = new_node; //will cause infinite loop 
        // }

        curr = this->insert_node_in_list(new_node);
        //curr = this->head;

    }
    return this->head;

}

Node* HuffmanListTree::insert_node_in_list(Node *new_node){ // make sure to do it in sorted order acending
    Node *curr = this->head;
    bool node_inserted = false;
    
    if(this->head == nullptr){
        this->head = new_node;
        this->last = new_node;
        return this->head;
    }
    if(new_node->weight <= this->head->weight){
        Node *hold = curr->prev_in_list;
        this->head->prev_in_list = new_node;
        new_node->next_in_list = this->head;
        this->head = new_node;
        return this->head;
    }
    
    
    while(curr->next_in_list != nullptr){
        
        if(new_node->weight <= curr->next_in_list->weight){
            Node *hold = curr->next_in_list;
            new_node->prev_in_list = curr;
            new_node->next_in_list = hold;
            curr->next_in_list = new_node;
            if(hold != nullptr ) hold->prev_in_list = new_node;
            node_inserted = true;
            break;      
        } 
        curr = curr->next_in_list;
    }

    if(curr->next_in_list == nullptr && !node_inserted){
        Node *hold = curr->next_in_list;
        new_node->prev_in_list = curr;
        curr->next_in_list = new_node;
    }
    return this->head;
}

void HuffmanListTree::detach_node_from_list(Node *n){
    Node *hold_next = n->next_in_list;
    Node *hold_prev = n->prev_in_list;
    if(hold_next != nullptr) hold_next->prev_in_list = hold_prev;
    if(hold_prev != nullptr) hold_prev->next_in_list = hold_next;
    n->prev_in_list = nullptr;
    n->next_in_list = nullptr;

    if(n == this->head) this->head = hold_next;
}



vector<tuple<char, uint8_t>> HuffmanListTree::generate_huffman_codes(){
    //the length of the code is dependent on the dataset to encode and the depth of the tree
    //for example if we are encoding the ASCII alphabet and its punctuations the dataset will not 
    //exeed 255 or one byte and thus the maximum depth a tree can have is 255 thus the code will 
    //not be bigger than 8 bits (aka one byte)

    this->gen_huff_code_helper(this->head, 0, 0xFF);
    //remove leading 1
    for(int i =0; i < this->huff_codes.size(); ++i){
        char c;
        uint8_t code;
        std::tie(c, code) = this->huff_codes.at(i);

        tuple<char, uint8_t> t;
    }

    return this->huff_codes;

}

void HuffmanListTree::gen_huff_code_helper(Node *curr_head, uint8_t curr_code, uint8_t code){
    if(curr_head == nullptr){
        return;
    }
    if(code == 0xFF){//0xff means we are just starting 
        //set a leading one to know when the code starts and ends, later we will ignore this 
        //for example a huffman code of 000 is just 0 to th computer and we cant tell how many 
        //zeros are needed to complete the hufman code for a given data point so I added a leading 
        //1 that can be ignored to signal the start of a code.
        curr_code= 1;
        code = 0;
    } else {
        curr_code <<= 1;
    }
    
    if(code == 1) curr_code |= 1;
    if(!(curr_head->ignore_data)){
        this->huff_codes.push_back({curr_head->data ,curr_code});
    }
    gen_huff_code_helper(curr_head->left_tree, curr_code, 0x0);
    gen_huff_code_helper(curr_head->right_tree, curr_code, 0x1);
}


void HuffmanListTree::recreate_huffman_tree(vector<tuple<char, uint8_t>> codes){
    this->huff_codes = codes;
    this->head = new Node('\0', -1, true);

    for(auto &hc: codes){
        char c;
        uint8_t code, code_norm, sig_dig;
        tie(c, code) = hc;
        sig_dig = get_sig_digs_in_code(code);
        this->recreate_huff_tree_helper(this->head, c, code, 1 << (sig_dig - 1));
    }
    
}
void HuffmanListTree::recreate_huff_tree_helper(Node *curr, char data, uint8_t code, uint8_t bit_idx){
    bool bit_set = code & bit_idx;

    if(bit_idx == 1){
        Node *leaf = new Node(data, code);
        if(!(curr->left_tree) && !bit_set) curr->left_tree = leaf;
        if(!(curr->right_tree) && bit_set) curr->right_tree = leaf;
        return;
    }

    Node *n = new Node('\0', -1, true);
    if(!bit_set){
        if(!(curr->left_tree)) curr->left_tree = n;
        this->recreate_huff_tree_helper(curr->left_tree, data, code, bit_idx>>1);
    }
    if(bit_set){
        if(!(curr->right_tree)) curr->right_tree = n;
        this->recreate_huff_tree_helper(curr->right_tree, data, code, bit_idx>>1);
    }
}

uint8_t HuffmanListTree::get_sig_digs_in_code(uint8_t code){
    uint8_t new_code = code, sig_dig = 0;
    while(code){
        code >>= 1;
        sig_dig += 1;
    }
    //uint8_t leading_bit = 1 << (sig_dig - 1);
    return sig_dig-1;  //minus the leading bit
}



void HuffmanListTree::print_huff_codes(){
    printf("Printing Huffman Codes\n");
    for(tuple<char, uint8_t> cc: this->huff_codes){
        char c;
        uint8_t code;
        std::tie(c, code) = cc;
        printf("<%c, %X>, ", c, code);
    }
    printf("\n");
}

void HuffmanListTree::print_list(){

    Node *curr = this->head;
    while(curr != nullptr){
        printf("<%c, %d>, ", curr->data, curr->weight);
        curr = curr->next_in_list;
    }
    printf("\n");
}

void HuffmanListTree::print_tree(Node *curr_head){
    if(curr_head == nullptr){
        return;
    }
     if(!(curr_head->ignore_data)){
        printf("<%c, %d>\n", curr_head->data, curr_head->weight);
    } else {
        printf("<NODE, %d>\n", curr_head->weight);
    }
    print_tree(curr_head->left_tree);
    print_tree(curr_head->right_tree);
    
}

void HuffmanListTree::free_tree(Node *curr_head){
    if(curr_head == nullptr){
        return;
    }
    free_tree(curr_head->left_tree);
    free_tree(curr_head->right_tree);
    free(curr_head);
    curr_head = nullptr;
}


void HuffmanListTree::free_list(){
    while(this->head){
        Node *new_head = this->head->next_in_list;
        free(this->head);
        this->head = new_head;
    }
    this->head = nullptr;
}
