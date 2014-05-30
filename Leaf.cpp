
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Leaf: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Leaf.h"

Leaf::Leaf(int v): Node(), leaf_list_ptr(NULL){
    value = v;
    type = leafnode;
}
Leaf::Leaf(Node *p, int v): leaf_list_ptr(NULL){
    parent = p;
    value = v;
    depth = p->get_depth() +1;
    type = leafnode;
}

Leaf::~Leaf(){
    //follow pointer to the leaflist entry to null it's pointer
    *leaf_list_ptr = NULL;
    leaf_list_ptr = NULL;
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