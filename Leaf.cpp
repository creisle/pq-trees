
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Leaf: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Leaf.h"

static bool test_leaks = false;
static std::list<Leaf*> leaflist;

Leaf::Leaf(Node *p, int v): Node(){
    if(test_leaks){ printf("LEAF ++\n"); }
    parent = p;
    value = v;
    depth = p->get_depth() +1;
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

void Leaf::print_expression(bool print_mark /*false*/){
    if(print_mark){
        switch(node_mark){
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

std::list<Leaf*> Leaf::mark_pertinent(int v){
    //printf("mark_pertinent()\n");
    std::list<Leaf*> fulls;
    std::list<Leaf*>::iterator it=leaflist.begin();
    while(it!=leaflist.end()){
        if((*it)==NULL){
            it = leaflist.erase(it);
        }else{
            if((*it)->get_value()==v){
                (*it)->mark();
                fulls.push_back((*it));
            }
            ++it;
        }
    }
    return fulls;
}

std::list<Leaf*> Leaf::get_pertinent(){
    std::list<Leaf*> fulls;
    std::list<Leaf*>::iterator it=leaflist.begin();
    while(it!=leaflist.end()){
        if((*it)==NULL){
            it = leaflist.erase(it);
        }else{
            if((*it)->get_mark()==full){
                fulls.push_back((*it)); 
            }
            ++it;
        }
    }
    return fulls;
}


