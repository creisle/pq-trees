
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

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++11-extensions"

static bool follow = false; //use this to find bugs. prints out function names when a function is executed
static bool debug = false;

static std::string string_marking[] = {"empty", "partial", "full"};

using std::cout;
using std::endl;
using std::cerr;

PQTree::PQTree()
{
    root = NULL;
    stage = 0;
}

PQTree::PQTree(std::vector<int> leaves, int src/*-1*/)
{
    root = new PQnode(leaves, leaflist, src);
    stage = 0;
}

PQTree::PQTree(std::string expr)
{
    size_t i=0;
    if(PQnode *tmp = dynamic_cast<PQnode*>(build_from_expr(expr, i)))
    {
        root = tmp;
    }else
    {
        root = NULL;
        cerr << "cannot initialize a pq tree with an invalid expression" << endl;
        exit(1);
    }
    stage = 0;
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


bool PQTree::reduce_and_replace(int v, std::vector<int> tree_in)
{
    std::list<int> sources;
    return reduce_and_replace(v, tree_in, sources);
}

bool PQTree::reduce_and_replace(int v, std::vector<int> tree_in, std::list<int> &sources)
{
    if(follow||debug){ cerr << "PQTree::reduce_and_replace(int value, std::vector<int> tree_in)" << endl; }
    //put the value inside a vector
    std::vector<int> values;
    values.push_back(v);
    
    PQnode* subroot = reduce(values); //pertinent subroot
    if(debug&&subroot!=NULL)
    {
        cerr << "current tree: " << print_expression(option_marking) << endl;
        cerr << "pertinent subroot: " << subroot->print_expression(option_marking) << endl;
    }
    if(subroot==NULL){
        if(follow||debug)
        {
            cerr << print_expression() << "; could not find the appropriate subroot. reduce failed" << endl;
        }
        //
        return false;
    }
    
    Node *child;
    if(tree_in.size()==1)
    {
        child = new Leaf(tree_in[0], leaflist, v);
    }else
    {
        child = new PQnode(tree_in, leaflist, v);
    }
    //subroot->unmark();
    
    if( !replace_full_with(child, sources) )
    {
        return false;
    }
    subroot->unmark();
    
    return true;
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
    
    if(follow){ cerr << "PQTree::reduce(int value). marked as " << print_expression(option_marking) << endl; }
    if(subroot!=NULL)
    {
        
        if(debug)
        {
            cerr << "pertinent subroot before reduce: " << subroot->print_expression(option_marking) << endl;
        }
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
        if(debug)
        {
            cerr << "pertinent subroot not found, could be invalid leaf values" << endl;
        }
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
bool PQTree::replace_full_with(Node *child, std::list<int> &sources)
{
    if(follow){ printf("PQTree::replace_full_with(Node *child)\n"); }
    
    if(child==NULL){ return false; }

    PQnode* parent = find_full_subroot();
    
    if(debug)
    {
        cerr << "\"full\" subroot: " << parent->print_expression() << endl;
    }
    
    if(parent==NULL){
        fprintf(stderr, "could not find the full subroot to replace\n");
        return false;
    }
    
    if(!parent->condense_and_replace(child, sources))
    {
        fprintf(stderr, "condense failed\n");
        return false;
    }
    if(parent->count_children()<3)
    {
        parent->set_type(pnode);
    }
    return true;
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
    if(follow){ printf("PQTree::mark(vector<int> v)\n"); }
    
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
        if(!(curr->mark())) //mark the node
        {
            return NULL;
        }
        if(PQnode *p = dynamic_cast<PQnode*>(curr->get_parent()))
        {
            partials.pop_front(); //remove the curr node and destroy the reference
            add_unique_by_depth(p, partials);
        }
        else //could be that the parent was null in which case just remove
        {
            partials.pop_front();
        }
        
    }
    if(!partials.empty() && (partials.front()->mark()) )
    {
        return partials.front();
    }
    return NULL;
    
}

PQnode* PQTree::find_full_subroot()
{
    if(follow){ printf("PQTree::find_full_subroot)\n"); }
    if(debug){ cerr << "find_full_subroot(): current tree: " << print_expression(option_marking) << endl; }
    std::list<Leaf*> fulls = get_pertinent(); //get the full leaves
    
    
    std::list<PQnode*> parents; 
    
    for(auto k = fulls.begin(); k!=fulls.end(); ++k)
    {
        PQnode *p = dynamic_cast<PQnode*>((*k)->get_parent());
        //add it into the partials list by inserting it at the correct position based on decreasing depth
        add_unique_by_depth(p, parents);
    }
    fulls.clear();
    
    //at this point we have a list of potential partials sorted by depth with no duplicates
    //now we need to mark these nodes.... and then their parents until there is only one node left in the partials list
    while(parents.size()>1)
    {
        PQnode *curr = parents.front();//always deal with the front element first
        PQnode *p = dynamic_cast<PQnode*>(curr->get_parent()); 
        parents.pop_front(); //remove the curr node and destroy the reference
        add_unique_by_depth(p, parents);
    }
    if(!parents.empty())
    {
        return parents.front();
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
Node* PQTree::build_from_expr(std::string &expr, size_t &i)
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
                    while(i<expr.length()&&expr[i]!=' '&&expr[i]!=']'&&expr[i]!='}')
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
    if(rt!=NULL)
    {
        if(isqnode&&rt->count_children()>2)
        {
            rt->set_type(qnode);
        }else if(rt->count_children()==1)
        {
            return lf;
        }
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
            if(mark)
            {
                cout << string_marking[(*it)->get_mark()] << ":" << (*it)->get_value() << endl;
            }
            else
            {
                printf("%d ", (*it)->get_value());
            }
        }
    }
    printf("\n");
}

std::string PQTree::convert_to_gml()
{
    int id = 0;
    std::string result = "graph [\n";
    result += root->convert_to_gml(id);
    result += "]\n";
    return result;
}




