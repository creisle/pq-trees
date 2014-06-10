/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Node.h"


//this will be the parent class for P-nodes, C-nodes and Q-nodes
//on the reduction step will ensure that we have a valid PQ tree
Node::Node() //default constructor
{ 
    parent = NULL;
    depth = 0;
    node_mark = empty;
}

//this procedure is just for testing purposes
// paritally abstract?
void Node::print()
{
    printf("Node addr: %p\n", this);
    if(parent==NULL)
    {
        printf("parent addr: NULL\n");
    }else
    {
        printf("parent addr: %p \n", parent);
    }
    switch(node_mark)
    {
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
    printf("depth = %d\n", depth);
    
}
        
//getter funtions
Node* Node::get_parent(){ return parent; }
int Node::get_depth() const{ return depth; }
nodetype Node::get_type() const{ return type; }
marking Node::get_mark() const{ return node_mark; }
void Node::unmark(){ node_mark = empty; }
void Node::set_parent(Node *p){ parent = p; }

//comparison to sort lists of marked nodes by marking so that they follow the form e...p...f
bool compare_marking(Node *first, Node *second)
{
    marking a = first->get_mark();
    marking b = second->get_mark();
    if(a==b)
    {
        return false;
    }else if(b==full) // b = full and a!=b
    { 
        return true;
    }else if(b==partial) // b = partial and a!=b
    { 
        if(a==full)
        {
            return false;
        }else
        {
            return true;
        }
    }else // b = empty, and a!=b
    { 
        return false;
    }
}


