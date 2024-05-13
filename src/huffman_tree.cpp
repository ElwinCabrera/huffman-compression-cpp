#include "../include/huffman_tree.h"

Node::Node(char data, int weight): data(data), weight(weight), next_in_list(nullptr), prev_in_list(nullptr), left_tree(nullptr), right_tree(nullptr), is_weight_node_only(false){

}

HuffmanListTree::HuffmanListTree(): head(nullptr), last(nullptr){

}

void  HuffmanListTree::add_to_list(char c, int weight){
    Node *new_node = new Node(c, weight);
    this->insert_node_in_list(new Node(c, weight));
}

void  HuffmanListTree::sort_list(){

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

Node* HuffmanListTree::build_tree(){

    Node *curr = this->head;
    while(curr != nullptr && curr->next_in_list != nullptr){
        Node *left = curr;
        Node *right = curr->next_in_list;
        Node *next = curr->next_in_list->next_in_list;
        this->detach_node_from_list(left);
        this->detach_node_from_list(right);


        Node *new_node = new Node('\0', left->weight + right->weight);
        new_node->is_weight_node_only = true;
        new_node->left_tree = left;
        new_node->right_tree = right;

        this->head = next;
        curr = next;

        this->insert_node_in_list(new_node);

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


void HuffmanListTree::print(){

    Node *curr = this->head;
    while(curr != nullptr){
        printf("<%c, %d>, ", curr->data, curr->weight);
        curr = curr->next_in_list;
    }
    printf("\n");
}

