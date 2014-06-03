
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Leaf: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Leaf.h"

Leaf::Leaf(Node *p, int v): Node(), leaf_list_ptr(NULL){
    printf("LEAF ++\n");
    parent = p;
    value = v;
    depth = p->get_depth() +1;
    type = leafnode;
    mark = empty;
}

Leaf::~Leaf(){
    //follow pointer to the leaflist entry to null it's pointer
    printf("LEAF -- \n");
    if(leaf_list_ptr!=NULL){
        *leaf_list_ptr = NULL;
        leaf_list_ptr = NULL;
    }
    
}

void Leaf::print(){
    printf("========= node-type: LEAF NODE ======== \n");
    Node::print();
    printf("value: %d\n\n", value);
}
void Leaf::set_mark(marking m){ mark = m;}
int Leaf::get_value(){ return value; }
void Leaf::set_leaf_list_ptr(Leaf** ptr){ leaf_list_ptr = ptr; }
Leaf** Leaf::get_leaf_list_ptr(){ return leaf_list_ptr; }

void Leaf::unmark(){ mark = empty; }

void Leaf::print_expression(bool m /*false*/){
    if(m){
        switch(mark){
            case full:
                printf("f:");
                break;
            case partial:
                printf("p:");
                break;
            default:
                printf("e:");
                break;
        }
    }
    printf("%d", value);
}

void Leaf::update_depth(){
    if(parent==NULL){
        depth = 0;
    }else{
        depth = parent->get_depth() + 1;
    }
}


