
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class Leaf: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Leaf.h"

static bool leaks = true;
static int buildcount = 0;

using std::cerr;
using std::endl;


Leaf::Leaf(Node *p, int v, std::list<Leaf*> &leaflist, int src /*-1*/)
    : Node()
{
    if(leaks){ ++buildcount; }
    parent = p;
    value = v;
    source = src;
    depth = p->get_depth() +1;
    leaflist.push_back(this);
    leaf_list_ptr = &leaflist.back();
    type = leafnode;
}

Leaf::Leaf(int v, std::list<Leaf*> &leaflist, int src /*-1*/)
    : Node()
{
    if(leaks){ ++buildcount; }
    value = v;
    source = src;
    leaflist.push_back(this);
    leaf_list_ptr = &leaflist.back();
    type = leafnode;
}


Leaf::~Leaf()
{
    //follow pointer to the leaflist entry to null it's pointer
    if(leaks){ --buildcount; }
    if(leaf_list_ptr!=NULL)
    {
        *leaf_list_ptr = NULL;
        leaf_list_ptr = NULL;
    }
    
}

bool Leaf::less_than(Node& other)
{
    if(Leaf *lf = dynamic_cast<Leaf *>(&other))
    {
        if(lf->value>value)
        {
            return true;
        }
    }
    else
    {
        return true;
    }
    return false;
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
int Leaf::get_source(){ return source; }

std::string Leaf::print_expression(print_option print_mark /*option_none*/)
{
    std::string result = "";
    switch(print_mark)
    {
        case option_marking:
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
            break;
        case option_depth:
            result += std::to_string(depth)+":";;
            break;
        case option_src:
            result += std::to_string(source)+":";
            break;
        default:
            break;
    }
    result += std::to_string(value);
    return result;
}

void Leaf::update_depth()
{
    if(parent==NULL)
    {
        depth = 0;
    }else
    {
        depth = parent->get_depth() + 1;
    }
}

std::string Leaf::convert_to_gml(int &id)
{
    std::string result = Node::convert_to_gml(id);
    result += "label \"";
    result += std::to_string(value);
    result += "\"\n]\n";
    return result;
}

    


