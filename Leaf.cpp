
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Leaf: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Leaf.h"

static bool test_leaks = false;


Leaf::Leaf(Node *p, int v, std::list<Leaf*> &leaflist): Node(){
    if(test_leaks){ printf("LEAF ++\n"); }
    parent = p;
    value = v;
    depth = p->get_depth() +1;
    leaflist.push_back(this);
    leaf_list_ptr = &leaflist.back();
}

Leaf::Leaf(int v, std::list<Leaf*> &leaflist): Node(){
    if(test_leaks){ printf("LEAF ++\n"); }
    parent = NULL;
    value = v;
    leaflist.push_back(this);
    leaf_list_ptr = &leaflist.back();
}


Leaf::~Leaf(){
    //follow pointer to the leaflist entry to null it's pointer
    if(test_leaks){  printf("LEAF -- \n"); }
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
void Leaf::mark(){ node_mark = full;}
int Leaf::get_value(){ return value; }
void Leaf::unmark(){ node_mark = empty; }

std::string Leaf::print_expression(bool print_mark /*false*/){
    std::string result = "";
    if(print_mark){
        switch(node_mark){
            case full:
                result += "f:";
                break;
            case partial:
                result += "p:";
                break;
            case empty:
                result += "e:";
                break;
        }
    }
    result += std::to_string(value);
    return result;
}

void Leaf::update_depth(){
    if(parent==NULL){
        depth = 0;
    }else{
        depth = parent->get_depth() + 1;
    }
}

bool contains(std::vector<int> vec, int v){
    for(size_t i=0; i<vec.size(); ++i){
        if(v==vec[i]){
            return true;
        }
    }
    return false;
}

    


