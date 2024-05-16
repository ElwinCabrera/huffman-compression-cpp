#include "../include/huffman_tree.h"

Node::Node(char data, int weight, bool ignore_data): data(data), weight(weight), next_in_list(nullptr), prev_in_list(nullptr), left_tree(nullptr), right_tree(nullptr), ignore_data(ignore_data){
}



HuffmanListTree::HuffmanListTree(): head(nullptr), tree_depth(0) { }


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

Node* HuffmanListTree::insert_node_in_list(Node *new_node){ // make sure to do it in sorted order acending
    Node *curr = this->head;
    bool node_inserted = false;
    
    if(this->head == nullptr){
        this->head = new_node;
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

void HuffmanListTree::print_list(){

    Node *curr = this->head;
    while(curr != nullptr){
        printf("<%c, %d>, ", curr->data, curr->weight);
        curr = curr->next_in_list;
    }
    printf("\n");
}

void HuffmanListTree::free_list(){
    while(this->head){
        Node *new_head = this->head->next_in_list;
        free(this->head);
        this->head = new_head;
    }
    this->head = nullptr;
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

void HuffmanListTree::recreate_huffman_tree(unordered_map<char, BitSequence> codes){
    this->huff_codes = codes;
    this->head = new Node('\0', -1, true);

    for(auto& [ch, code]: codes){
        this->recreate_huff_tree_helper(this->head, ch, code, code.get_num_bits() - 1);
    }
    
}
void HuffmanListTree::recreate_huff_tree_helper(Node *curr, char data, BitSequence bit_sequence, uint64_t bit_idx){
    bool bit_set = bit_sequence.get_bit(bit_idx);

    if(bit_idx == 0){
        Node *leaf = new Node(data, bit_sequence.get_x_bytes(8));
        if(!bit_set && !(curr->left_tree) ) curr->left_tree = leaf;
        if(bit_set  && !(curr->right_tree) ) curr->right_tree = leaf;
        return;
    }

    Node *n = new Node('\0', -1, true);
    if(!bit_set){
        if(!(curr->left_tree)) curr->left_tree = n;
        this->recreate_huff_tree_helper(curr->left_tree, data, bit_sequence, --bit_idx);
    }
    if(bit_set){
        if(!(curr->right_tree)) curr->right_tree = n;
        this->recreate_huff_tree_helper(curr->right_tree, data, bit_sequence, --bit_idx);
    }
}


uint8_t* HuffmanListTree::decode_bit_seq(BitSequence bit_sequence){
    uint8_t *data = nullptr;

    //there might be a better way of finding the length 
    //(we could use a 'string' instead of 'uint8_t*' which will dynamically allocates more space for us)
    //but for now this will do
    bit_sequence.get_next_bit_start(0);
    int data_len = this->find_data_len_from_bit_seq(this->head, 0, &bit_sequence);

    data = (uint8_t*) malloc(data_len + 1);
    memset(data, '\0', data_len);

    uint8_t *data_ptr = data;
    bit_sequence.get_next_bit_start(0);
    this->decode_bit_seq_helper(this->head, data_ptr, &bit_sequence);

    return data;
}

void HuffmanListTree::decode_bit_seq_helper(Node* curr, uint8_t *buff, BitSequence *bit_sequence){
    if(!curr || bit_sequence->get_next_bit_idx() >= bit_sequence->get_num_bits()){
        return;
    }
    if(!(curr->ignore_data)){
        *buff++ = (uint8_t) curr->data;
        curr = this->head;
    }
    bool bit_set = bit_sequence->get_next_bit();
    if(!bit_set){
        this->decode_bit_seq_helper(curr->left_tree, buff, bit_sequence);
    } else{
        this->decode_bit_seq_helper(curr->right_tree, buff, bit_sequence);
    }
}

int HuffmanListTree::find_data_len_from_bit_seq(Node* curr, int count, BitSequence *bit_sequence){
    if(!curr || bit_sequence->get_next_bit_idx() >= bit_sequence->get_num_bits()){
        return count;
    }
    if(!(curr->ignore_data)){
        count += 1;
        curr = this->head;
    }
    bool bit_set = bit_sequence->get_next_bit();
    if(!bit_set){
        count = this->find_data_len_from_bit_seq(curr->left_tree, count, bit_sequence);
    } else{
        count = this->find_data_len_from_bit_seq(curr->right_tree, count, bit_sequence);
    }

    return count;
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









unordered_map<char, BitSequence> HuffmanListTree::generate_huffman_codes(){
    //the length of the code is dependent on the dataset to encode and the depth of the tree
    //for example if we are encoding the ASCII alphabet and its punctuations the dataset will not 
    //exeed 255 or one byte and thus the maximum depth a tree can have is 255 thus the code will 
    //not be bigger than 8 bits (aka one byte)

    this->gen_huff_code_helper(this->head, 0, 0, false);

    return this->huff_codes;

}

void HuffmanListTree::gen_huff_code_helper(Node *curr_head, uint64_t depth, uint64_t curr_code, bool set){
    if(curr_head == nullptr){
        return;
    }

    
    curr_code <<= 1;
    if(set) curr_code |= 1;

    if(this->huff_codes.find(curr_head->data) != this->huff_codes.end()){
        printf("woops we accidentally visited same data twice\n");
    }

    if(depth > this->tree_depth) this->tree_depth = depth;

    if(!(curr_head->ignore_data)){
        uint64_t tmp_code = curr_code;
        int idx = depth - 1;

        BitSequence bit_seq(depth);
        bit_seq.set_bits_from_num(0, curr_code);

        this->huff_codes[curr_head->data] = bit_seq;
    }

    gen_huff_code_helper(curr_head->left_tree,  depth+1, curr_code, false);
    gen_huff_code_helper(curr_head->right_tree, depth+1, curr_code, true);
}

void HuffmanListTree::print_huff_codes(){
    printf("Printing Huffman Codes\n");
    for(auto& [ch, code]: this->huff_codes){
        uint64_t cd = code.get_x_bytes(8);
        if(cd == 0){
            printf("<%c, %X(x%d)>, ", ch, cd, code.get_num_bits());
        } else {
            printf("<%c, %X>, ", ch, cd);
        }
        
    }
    printf("\n");
}




