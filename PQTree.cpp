
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

PQTree::PQTree(std::vector<int> leaves)
{
    root = new PQnode(leaves, leaflist);
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
bool PQTree::reduce_and_replace(int v, std::vector<int> tree_in)
{
    if(follow){ printf("PQTree::reduce_and_replace(int value, std::vector<int> tree_in)\n"); }
    
    //put the value inside a vector
    std::vector<int> values;
    values.push_back(v);
    
    PQnode* subroot = reduce(values); //pertinent subroot
    if(subroot==NULL){ return false; }
    
    Node *child;
    if(tree_in.size()==1)
    {
        child = new Leaf(tree_in[0], leaflist);
    }else
    {
        child = new PQnode(tree_in, leaflist);
    }
    if(!replace_full_with(child)){ return false; }
    
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
    if(follow){ printf("PQTree::reduce(int value). marked as "); print_expression(true); }
    if(subroot!=NULL)
    {
        if(subroot->reduce())
        {
            return subroot;
        }
    }else
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
bool PQTree::replace_full_with(Node *child)
{
    if(follow){ printf("PQTree::replace_full_with(Node *child)\n"); }
    
    Node* parent = NULL;
    if(child==NULL){ return false; }
    std::list<Leaf*> fulls = get_pertinent();
    
    for(std::list<Leaf*>::iterator it=fulls.begin(); it!=fulls.end(); ++it)
    {
        if(parent)
        {
            if((*it)->get_parent()!=parent)
            {
                fprintf(stderr, "not all full nodes have the same parent\n");
                return false;
            }
        }else
        {
            parent = (*it)->get_parent();
        }
    }
    
    if(PQnode* temp = dynamic_cast<PQnode*>(parent))
    {
        if(!(temp->condense_and_replace(child)))
        {
            fprintf(stderr, "condense failed\n");
            return false;
        }
        if(temp->count_children()<3)
        {
            temp->set_type(pnode);
        }
        
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
    if(partials.empty())
    {
        partials.push_back(p);
    }else
    {
        for(std::list<PQnode*>::iterator it=partials.begin(); it!=partials.end(); ++it) //iterates through our list of partials
        { 
            if((*it)->get_depth()>=p->get_depth())
            {
                if(p==(*it)){ break; }
            }else
            {
                partials.insert(it, p);
            }
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: print_expression(bool mark)
 * input: boolean value determining whether or not to print out the more verbose expression
 *      string that also gives the marking "value" for each node or leaf in the tree
 * purpose: prints out an expression corresponding to the current tree structure
 * returns: the expression string
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string PQTree::print_expression(bool mark/*false*/)
{
    std::string result = root->print_expression(mark);
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

//calls the recusive tree compare method in the PQnode class
bool PQTree::equivalent(PQTree &tree)
{
    printf("equivalent(tree)\n");
    if( get_leaflist_size()!=tree.get_leaflist_size() ) //must have the same number of leaves or immediately reject
    {
        return false;
    }
    
    std::multiset<int> a, b; //push the leaf values into multisets
    std::list<custom::descendant_set> plist, foreign_plist;
    
    for(auto it: leaflist)
    {
        a.insert(it->get_value());
    }
    for(auto it: tree.leaflist)
    {
        b.insert(it->get_value());
    }
    if(a!=b) //must have the same elements in the leaflist
    {
        return false;
    }
    
    //add parents of the leaves and create the list of partitions
    for(auto it: leaflist)
    {
        custom::descendant_set curr;
        (curr.leaves).insert(it->get_value());
        curr.used = false;
        if((curr.p = dynamic_cast<PQnode*>(it->get_parent())))
        {
            update_ancestor_list(plist, curr);
        }
    }
    
    for(auto it: tree.leaflist)
    {
        custom::descendant_set curr;
        (curr.leaves).insert(it->get_value());
        curr.used = false;
        if((curr.p = dynamic_cast<PQnode*>(it->get_parent())))
        {
            update_ancestor_list(foreign_plist, curr);
        }
    }
    
    int current_depth = ((plist.front()).p)->get_depth();
    if(current_depth!=((foreign_plist.front()).p)->get_depth())
    {
        return false;
    }
    printf("after dealing with the leaves this tree has %lu parents and the potential equivalent has %lu parnets\n", plist.size(), foreign_plist.size());
    return equivalent(current_depth, plist, foreign_plist);
}

//add in the correct positioning based on the depth
//if not unique, merge the leaf values from the element we are on currently
//we just removed curr from the front of the list and we are on it's depth level
bool PQTree::update_ancestor_list(std::list<custom::descendant_set> &plist, custom::descendant_set curr)
{
    printf("update_ancestor_list()\n");
    if(plist.empty())
    {
        curr.used = false;
        plist.push_back(curr);
        return true;
    }
    
    auto it = plist.begin();
    while(it!=plist.end())
    {
        if((*it).p==NULL)
        {
            return false;
        }
        if(((*it).p)->get_depth()<(curr.p)->get_depth()) //too far, won't be in the list, add it here
        {
            break;
        }
        else if((*it).p==(curr.p)) //point to the same node, merge the value sets
        {
            ((*it).leaves).insert((curr.leaves).begin(), (curr.leaves).end());
            return true;
        } 
        ++it;
    }
    curr.used = false;
    plist.insert(it, curr);
    return true;
    
}

//incoming list items must be sorted wrt to depth? decresing. highest depth param first aka lowest on the tree
bool PQTree::equivalent(int current_depth, std::list<custom::descendant_set> plist, std::list<custom::descendant_set> foreign_plist) //already dealt with the leaves for each
{
    printf("equivalent(depth, list1, list2)\n");
    
    
    if(plist.size()!=foreign_plist.size()) //should always be the same size since we deal with one depth level at a time
    {
        return false;
    }
    else if(current_depth==0) //when we get to here, all elements must have been merged into the multiset
    {
        custom::descendant_set a = plist.front();
        custom::descendant_set b = foreign_plist.front();
        printf("we are at the root\n");
        (a.p)->print();
        (b.p)->print();
        
        if( a.leaves==b.leaves && (a.p)->get_type()==(b.p)->get_type() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    if(((plist.front()).p)->get_depth()!=current_depth)
    {
        return false;
    }
    
    //deal with all the elements in the plist at the current depth level
    
    for(auto it: plist)
    {
        if((it.p)->get_depth()!=current_depth) //finished
        {
            break;
        }
        it.used=false;
        //see if this multiset is covered in the other list
        for(auto k: foreign_plist)
        {
            if((k.p)->get_depth()<current_depth)
            {
                break;
            }
            else if((k.p)->get_depth()==current_depth&&k.used!=true) //should be a one-to-one correspondence between the trees at all levels
            {
                if(k.leaves==it.leaves&&(k.p)->get_type()==(it.p)->get_type())
                {
                    it.used = true;
                    k.used = true;
                }
            }
        }
        
        if(it.used==false)
        {
            return false;
        }
    }
    
    //now update at this depth level
    auto it = plist.begin();
    while(it!=plist.end())
    {
        if(((*it).p)->get_depth()==current_depth)
        {
            custom::descendant_set temp = (*it);
            it = plist.erase(it);
            if(PQnode *par = dynamic_cast<PQnode*>((temp.p)->get_parent()))
            {
                temp.p = par;
                if(!update_ancestor_list(plist, temp)){ return false; }
            }
            else
            {
                return false;
            }
            
        }
        else
        {
            break;
        }
    }
    
    //now update at this depth level for the foreign list
    it = foreign_plist.begin();
    while(it!=foreign_plist.end())
    {
        if(((*it).p)->get_depth()==current_depth)
        {
            custom::descendant_set temp = (*it);
            it = foreign_plist.erase(it);
            if(PQnode *par = dynamic_cast<PQnode*>((temp.p)->get_parent()))
            {
                temp.p = par;
                if(!update_ancestor_list(foreign_plist, temp)){ return false; }
            }
            else
            {
                return false;
            }
        }
        else
        {
            break;
        }
    }
    --current_depth;
    
    return equivalent(current_depth, plist, foreign_plist);
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

//should the first node be sorted before the second node? false means do nothing, true means sort them
//are they in the correct order as first and second?
bool custom::compare(Node *first, Node *second)
{
    if( Leaf *lf = dynamic_cast<Leaf*>(first)) //first is a leaf
    {
        if( Leaf *tmp = dynamic_cast<Leaf*>(second))
        {
            if(lf->get_value()>tmp->get_value())
            {
                return false;
            }
        }
    }
    else if(Leaf *lf = dynamic_cast<Leaf*>(second)) //second is a leaf, first is not a leaf
    {
        return false;
    }
    else //neither is a leaf
    {
        
    }
    return true;
}



