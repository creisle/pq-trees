
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class PQTree: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Note: the leaflist list is referenced by other variables in the program. list nodes should
 * have the same ref throughout the program until they are destroyed
 * however if for some reason thier addr changes this would be a problem
 * but i really don't think this would happen
 */

#include "PQTree.h"
static bool follow = false; //use this to find bugs. prints out function names when a function is executed

PQTree::PQTree()
{
    root = NULL;
}

PQTree::PQTree(std::vector<int> leaves, int src/*-1*/)
{
    root = new PQnode(leaves, leaflist, src);
}

PQTree::PQTree(std::string const expr)
{
    size_t i=0;
    if(PQnode *tmp = dynamic_cast<PQnode*>(build_from_expr(expr, i)))
    {
        root = tmp;
    }else
    {
        root = NULL;
        fprintf(stderr, "cannot initialize a pq tree with an invalid expression\n");
        exit(1);
    }
}

PQTree::~PQTree()
{
    delete root;
    root = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: print()
 * input: none
 * purpose: recursively prints pertinent node information. for debugging
 * return: none
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQTree::print()
{
    if(root==NULL)
    {
        printf("Empty tree\n");
    }else
    {
        root->print();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: reduce(std::vector<int> values)
 * input: value which we wish to be consecutive and an input vector of ints representing
 *      the leaves of the new universal tree
 * purpose: performs reductions on the tree based on the input vector. after reduction
 *      replaces the full leaves with the new universal tree that was built from the
 *      input vector
 * return: false is an error occurs or the tree is irreducible, true otherwise
 * note: use this for planarity testing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::list<int> PQTree::reduce_and_replace(int v, std::vector<int> tree_in)
{
    if(follow){ printf("PQTree::reduce_and_replace(int value, std::vector<int> tree_in)\n"); }
    
    //put the value inside a vector
    std::vector<int> values;
    values.push_back(v);
    
    PQnode* subroot = reduce(values); //pertinent subroot
    if(subroot==NULL){
        fprintf(stderr, "%s; could not find the appropriate subroot\n", print_expression().c_str());
        std::list<int> srcs;
        return srcs;
    }
    
    Node *child;
    if(tree_in.size()==1)
    {
        child = new Leaf(tree_in[0], leaflist, v);
    }else
    {
        child = new PQnode(tree_in, leaflist, v);
    }
    
    std::list<int> srcs = replace_full_with(child);
    if(srcs.empty()){ return srcs; }
    subroot->unmark();
    
    return srcs;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: reduce(std::vector<int> values)
 * input: vector of values which we wish to be consecutive
 * purpose: performs reductions on the tree based on the input vector. does not add
 *      or remove any leaves from the tree
 * return: false is an error occurs or the tree is irreducible, true otherwise
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PQnode* PQTree::reduce(std::vector<int> values)
{
    if(follow){ printf("PQTree::reduce(int value)\n"); }
    
    PQnode* subroot = mark(values); //pertinent subroot
    //std::cout << "after marking: " << print_expression(option_marking) <<"\n";
    if(follow){ printf("PQTree::reduce(int value). marked as %s\n", print_expression(option_marking).c_str()); }
    if(subroot!=NULL)
    {
        if(subroot->get_mark()==full)
        {
            return subroot;
        }
        else if(subroot->reduce())
        {
            return subroot;
        }
    }
    else
    {
        fprintf(stderr, "pertinent subroot not found, could be invalid leaf values\n");
    }
    return NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: set_consecutive(std::vector<int> values)
 * input: vector of values which we wish to be consecutive
 * purpose: performs reductions on the tree based on the input vector. does not add
 *      or remove any leaves from the tree
 * return: false is an error occurs or the tree is irreducible, true otherwise
 * note: this is what we would use to find interval graphs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQTree::set_consecutive(std::vector<int> values)
{
    if(follow){ printf("PQTree::set_consecutive(std::vector<int> values)\n"); }
    PQnode *subroot = reduce(values);
    if(subroot==NULL){ return false; }
    
    subroot->unmark();
    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: replace_full_with(Node *child)
 * input: a pointer to the child node we want to insert
 * purpose: occurs after tree reduction. finds the common parent of all of the "full"
 *      leaves. removes the full leaves and replaces them with the input child node
 *      while conserving the position
 * return: false is an error occurs, true otherwise
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::list<int> PQTree::replace_full_with(Node *child)
{
    if(follow){ printf("PQTree::replace_full_with(Node *child)\n"); }
    std::list<int> srcs;
    
    Node* parent = NULL;
    if(child==NULL){ return srcs; }
    std::list<Leaf*> fulls = get_pertinent();
    
    for(std::list<Leaf*>::iterator it=fulls.begin(); it!=fulls.end(); ++it)
    {
        if(parent)
        {
            if((*it)->get_parent()!=parent)
            {
                fprintf(stderr, "%s, not all full nodes have the same parent\n", print_expression().c_str());
                return srcs;
            }
        }else
        {
            parent = (*it)->get_parent();
        }
    }
    
    if(PQnode* temp = dynamic_cast<PQnode*>(parent))
    {
        srcs.merge(temp->condense_and_replace(child));
        if(srcs.empty())
        {
            fprintf(stderr, "condense failed\n");
            return srcs;
        }
        if(temp->count_children()<3)
        {
            temp->set_type(pnode);
        }
    }
    //std::cout << "AFTER condesation: " << print_expression() << "\n";
    return srcs;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: mark(std::vector<int> v)
 * input: vector of values representing the Leaf values we wish to mark as full
 * purpose: marks the pertinent subtree
 * return: the pertinent subroot. returns NULL if an error occured or the current tree
 *      is irreducible
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
PQnode* PQTree::mark(std::vector<int> v)
{   
    if(follow){ printf("PQTree::mark(int v)\n"); }
    
    std::list<Leaf*> fulls = mark_pertinent(v); //mark the full leaves based on the input values
    std::list<PQnode*> partials; 
    
    for(std::list<Leaf*>::iterator k = fulls.begin(); k!=fulls.end(); ++k)
    {
        PQnode *p = dynamic_cast<PQnode*>((*k)->get_parent());
        if(p==NULL) //this is the parent we want to add to the list of potential partials
        {
            return NULL;
        }
        
        //add it into the partials list by inserting it at the correct position based on decreasing depth
        add_unique_by_depth(p, partials);
    }
    fulls.clear();
    
    //at this point we have a list of potential partials sorted by depth with no duplicates
    //now we need to mark these nodes.... and then their parents until there is only one node left in the partials list
    
    while(partials.size()>1)
    {
        PQnode *curr = partials.front();//always deal with the front element first
        //std::cout << "currently looking at the node: " << curr->print_expression(option_depth) << "\n";
        if(!(curr->mark())) //mark the node
        {
            return NULL;
        }
        PQnode *p = (PQnode*)curr->get_parent(); //any parent in the tree will never be a leaf since they cannot have children. therefore this casting is safe
        partials.pop_front(); //remove the curr node and destroy the reference
        add_unique_by_depth(p, partials);
    }
    if(!partials.empty())
    {
        if( !(partials.front()->mark()) )
        {
            return NULL;
        }
        return partials.front();
    }
    return NULL;
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: add_unique_by_depth(PQnode *p, std::list<PQnode*> &partials)
 * input: list of nodes we want to add to, pointer to the node we want to add
 * purpose: adds a node to the list iff it is not already in the list. inserts into the
 *      list and maintains the decreasing depth by property of the list
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQTree::add_unique_by_depth(PQnode *p, std::list<PQnode*> &partials){
    
    bool found = false;
    
    if(partials.empty())
    {
        partials.push_back(p);
        found = true;
    }
    else
    {
        for(std::list<PQnode*>::iterator it=partials.begin(); it!=partials.end()&&!found; ++it) //iterates through our list of partials
        {
            if(p->get_depth()<=(*it)->get_depth()) //(*it)->get_depth()>=p->get_depth()
            {
                if(p==(*it))
                {
                    found = true;
                }
            }
            else
            {
                found = true;
                partials.insert(it, p);
            }
        }
    }
    if(!found)
    {
        partials.push_back(p); //got to the end of the list
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: print_expression(bool mark)
 * input: boolean value determining whether or not to print out the more verbose expression
 *      string that also gives the marking "value" for each node or leaf in the tree
 * purpose: prints out an expression corresponding to the current tree structure
 * returns: the expression string
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string PQTree::print_expression(print_option p/*option_none*/)
{
    std::string result = root->print_expression(p);
    return result;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: build_from_expr(std::string const expr, size_t &i)
 * input: an expression representing a tree and the int representing the position in the string
 * purpose: buils a tree that corresponds to the input expression. {} indicates a pnode, []
 *      indicates qnode and int values indicate leaves. while leaves must be separated by
 *      whitepace to differentiate leaves, the amount of whitespace is irrelevant
 * returns: a Node pointer to the prospective "root". returns NULL if the expression is invalid
 * notes: this function is recursive
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Node* PQTree::build_from_expr(std::string const expr, size_t &i)
{
    if(follow){ printf("PQTree::build_from_expr(std::string const expr, int &i)\n"); }
    
    int state = 0;
    bool reading = true;
    bool isqnode = false;
    PQnode *rt = NULL;
    Leaf *lf = NULL;
    
    while(reading&&i<expr.length())
    {
        switch(state)
        {
            case 0:
                if(isspace(expr[i])) //ignore whitespace
                {
                    ++i;
                }else if(expr[i]=='{'||expr[i]=='[')
                {
                    if(expr[i]=='[')
                    {
                        isqnode = true;
                    }
                    state = 1; ++i; rt = new PQnode();
                }else
                {
                    return NULL;
                }
                break;
            case 1: //started a node. linking children
                if(isspace(expr[i])) //ignore whitespace
                { 
                    ++i;
                }else if(expr[i]=='{'||expr[i]=='[') //start the next pnode
                { 
                    Node *child = build_from_expr(expr, i);
                    rt->link_child(child);
                    ++i;
                }else if(expr[i]=='}'||expr[i]==']')
                {
                    reading = false;
                }else if(isdigit(expr[i]))
                {
                    //get the number terminated by a comma
                    std::string num = "";
                    while(expr[i]!=' '&&expr[i]!=']'&&expr[i]!='}'&&i<expr.length())
                    {
                        num += expr[i++];
                    }
                    lf = new Leaf(rt, atoi(num.c_str()), leaflist);
                    rt->link_child(lf);
                }
                break;
            default:
                return NULL;
        }
    }
    if(isqnode&&rt->count_children()>2)
    {
        rt->set_type(qnode);
    }else if(rt->count_children()==1)
    {
        return lf;
    }
    return rt;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: mark_pertinent(std::vector<int> vec)
 * input: a vector of int values
 * purpose: runs through the leaflist and marks any nodes with any one of the values in
 *      the input vector to full. returns a list of these leaves that are marked full
 *      simultaneously it performs a "lazy cleanup" on the list. i.e. removing any
 *      nodes in the list that are null pointers from previously removed leaves
 * returns a list of leaves
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::list<Leaf*> PQTree::mark_pertinent(std::vector<int> vec)
{
    if(follow){ printf("PQTree::mark_pertinent(std::vector<int> vec)\n"); }
    std::list<Leaf*> fulls;
    std::list<Leaf*>::iterator it=leaflist.begin();
    while(it!=leaflist.end())
    {
        if((*it)==NULL)
        {
            it = leaflist.erase(it);
        }else
        {
            if(custom::contains(vec, (*it)->get_value())){
                (*it)->mark();
                fulls.push_back((*it));
            }
            ++it;
        }
    }
    return fulls;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: get_pertinent()
 * purpose: runs through the leaflist and returns a list of leaves that are marked full
 *      simultaneously it performs a "lazy cleanup" on the list. i.e. removing any
 *      nodes in the list that are null pointers from previously removed leaves
 * returns a list of leaves
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::list<Leaf*> PQTree::get_pertinent()
{
    if(follow){ printf("PQTree::get_pertinent()\n"); }
    
    std::list<Leaf*> fulls;
    std::list<Leaf*>::iterator it=leaflist.begin();
    while(it!=leaflist.end())
    {
        if((*it)==NULL)
        {
            it = leaflist.erase(it);
        }else
        {
            if((*it)->get_mark()==full)
            {
                fulls.push_back((*it)); 
            }
            ++it;
        }
    }
    return fulls;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: get_leaflist_size()
 * purpose: returns the size of the current leaflist. mainly for testing
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
size_t PQTree::get_leaflist_size()
{
    return leaflist.size();
}

bool PQTree::equivalent(PQTree &other)
{
    root->sort();
    (other.root)->sort();
    return custom::compare(print_expression(), other.print_expression());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: compare(std::string s1, std::string s2)
 * purpose: compares two strings for equality ignoring spaces (' ')
 * returns false if they are not equal, true if they are
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool custom::compare(std::string s1, std::string s2)
{
    auto a = s1.begin();
    auto b = s2.begin();
    while(a!=s1.end())
    {
        if((*a)!=' ') //ignore whitespace in s1
        {
            while(b!=s2.end())
            {
                if((*b)!=' ') //ignore whitespace in s2
                {
                    if((*b)==(*a))
                    {
                        ++b;
                        break;
                    }else
                    {
                        return false;
                    }
                }
                ++b;
            }
        }
        ++a;
    }
    
    return true;
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

void PQTree::print_leaflist(bool mark)
{
    for(auto it=leaflist.begin(); it!=leaflist.end(); ++it)
    {
        if(*it==NULL)
        {
            printf("NULL ");
        }
        else
        {
            printf("%d ", (*it)->get_value());
        }
    }
    printf("\n");
}





