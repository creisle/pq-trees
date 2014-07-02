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
    type = pnode;
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: contains(std::vector<int> vec, int v)
 * input: a vector of int values and an int
 * purpose: checks if the int v is a member of the vector
 * returns false if the vector does not have the value specified, true otherwise
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool custom::contains(std::vector<int> vec, int v)
{
    for(size_t i=0; i<vec.size(); ++i)
    {
        if(vec[i]==v)
        {
            return true;
        }
    }
    return false;
}

std::string Node::convert_to_gml(int &id)
{
    ++id;
    std::string result = "node [\nid ";
    result += std::to_string(id);
    result += "\ngraphics [\nw 20.0\n";
    result += "h 20.0\n]\n";
    return result;
}

