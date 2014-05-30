/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Node.h"

//this will be the parent class for P-nodes, C-nodes and Q-nodes
//on the reduction step will ensure that we have a valid PQ tree
Node::Node(){ //default constructor
    parent = NULL;
    depth = 0;
    mark = empty;
}

//this procedure is just for testing purposes
// paritally abstract?
void Node::print(){
    printf("Node addr: %p\n", this);
    if(parent==NULL){
        printf("parent addr: NULL\n");
    }else{
        printf("parent addr: %p \n", parent);
    }
    switch(mark){
        case full:
            printf("mark: full\n");
            break;
        case empty:
            printf("mark: empty\n");
            break;
        case partial:
            printf("mark: partial\n");
            break;
    }
    
}
        
//getter funtions
Node* Node::get_parent(){ return parent; }
int Node::get_depth(){ return depth; }
nodetype Node::get_type(){ return type; }
marking Node::get_mark(){ return mark; }