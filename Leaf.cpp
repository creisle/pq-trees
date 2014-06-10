
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Leaf: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Leaf.h"

static bool test_leaks = false;


Leaf::Leaf(Node *p, int v, std::list<Leaf*> &leaflist)
    : Node()
{
    if(test_leaks){ printf("LEAF ++\n"); }
    parent = p;
    value = v;
    depth = 0;
    p->update_depth();
    leaflist.push_back(this);
    leaf_list_ptr = &leaflist.back();
    type = leafnode;
}

Leaf::Leaf(int v, std::list<Leaf*> &leaflist)
    : Node()
{
    if(test_leaks){ printf("LEAF ++\n"); }
    value = v;
    leaflist.push_back(this);
    leaf_list_ptr = &leaflist.back();
    type = leafnode;
}


Leaf::~Leaf()
{
    //follow pointer to the leaflist entry to null it's pointer
    if(test_leaks){  printf("LEAF -- \n"); }
    if(leaf_list_ptr!=NULL)
    {
        *leaf_list_ptr = NULL;
        leaf_list_ptr = NULL;
    }
    
}

void Leaf::print()
{
    printf("========= node-type: LEAF NODE ======== \n");
    Node::print();
    printf("value: %d\n\n", value);
}
void Leaf::mark(){ node_mark = full;}
int Leaf::get_value(){ return value; }
void Leaf::unmark(){ node_mark = empty; }

std::string Leaf::print_expression(print_option m/*none*/)
{
    std::string result = "";
    if(m==mark_option)
    {
        switch(node_mark)
        {
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
    else if(m==depth_option)
    {
        result += std::to_string(depth)+": ";
    }
    result += std::to_string(value);
    return result;
}

void Leaf::update_depth()
{
    depth = 0;
}

//test if two leaves are equivalent
bool Leaf::is_equivalent(Node *other)
{
    if(Leaf *lf = dynamic_cast<Leaf*>(other))
    {
        if(lf->get_value()==value)
        {
            return true;
        }
    }
    return false;
}



    


