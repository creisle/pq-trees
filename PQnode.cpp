

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class PQNode: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "PQnode.h"

static bool test_leaks = false;
static bool follow = false;

PQnode::PQnode()
    : Node()
{
    if(test_leaks){ printf("PQNODE ++\n");}
    type = pnode;
}

PQnode::PQnode(std::vector<int> leaves, std::list<Leaf*> &leaflist, nodetype t/*pnode*/)
    : Node()
{
    if(test_leaks){ printf("PQNODE ++\n");}
    
    for(size_t i=0; i<leaves.size(); ++i)
    {
        Leaf *lf = new Leaf(this, leaves[i], leaflist);
        children.push_back(lf);
    }
    depth = 1;
    set_type(t);
}

PQnode::~PQnode()
{
    if(test_leaks){ printf("PQNODE --\n");}
    
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it)
    {
        delete *it;
    }
    children.clear();
}

void PQnode::print()
{
    printf("node-type: %s\t", (type==pnode)? "P": "Q");
    Node::print();
    printf("num children: %lu : ", children.size());
    
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it)
    {
        if(Leaf *lf = dynamic_cast<Leaf*>((*it)))
        {
            printf("%d ", lf->get_value());
        }
        else if(PQnode *pq = dynamic_cast<PQnode*>((*it)))
        {
            printf("%c ", (pq->get_type()==pnode)? 'P': 'Q');
        }
        
    }
    printf("\n");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: skip_marks(std::list<Node*>::iterator &itr, marking m)
 * input:
 *      itr: iterator in the current node child list. current pos
 *      m: the mark we want to skip past
 * purpose:
 *      runs through the list from the current position in the child list until it hits
 *      a mark that isn't the specified type. updates the position of the iterator that
 *      is passed in
 * return:
 *      number of elements found with that marking
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
size_t PQnode::skip_marks(std::list<Node*>::iterator &itr, marking m)
{
    size_t count = 0;
    while(itr!=children.end())
    {
        if((*itr)->get_mark()==m)
        {
            ++count;
            ++itr;
        }
        else //stops when it finds any node that doesn't have the mark we expect
        {
            return count;
        }
    }
    return count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: grab_marks(std::list<Node*>::iterator &itr, marking m, std::list<Node*> &tmp)
 * input:
 *      itr: iterator in the current node child list. current pos
 *      m: the mark we want to skip past
 *      tmp: the node list where we will add all the nodes we pass
 * purpose:
 *      runs through the list from the current position in the child list adding to the input list
 *      until it hits a mark that isn't the specified type. updates the position of the iterator
 *      that is passed in
 * return:
 *      number of elements found with that marking
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
size_t PQnode::grab_marks(std::list<Node*>::iterator &itr, marking m, std::list<Node*> &tmp)
{
    size_t count = 0;
    while(itr!=children.end())
    {
        if((*itr)->get_mark()==m)
        {
            tmp.push_back((*itr));
            ++count;
            ++itr;
        }
        else //stops when it finds any node that doesn't have the mark we expect
        {
            return count;
        }
    }
    return count;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: grab_marks(std::list<Node*>::iterator &itr, marking m, std::list<Node*> &tmp)
 * input: none
 * purpose:
 *      check the children in order to mark the node
 * return:
 *      true if the node is reducible
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::mark()
{
    size_t ecount = 0;
    size_t pcount = 0;
    size_t fcount = 0;
    std::list<Node*>::iterator it;
    if(type==pnode)
    {
        sort_children(); //puts the children in order of increasign full nodes: emptys....partials....fulls    
        it = children.begin();
        ecount += skip_marks(it, empty);
        pcount += skip_marks(it, partial);
        fcount += skip_marks(it, full);
    }
    else //type = qnode
    {
        it = children.begin();
        ecount += skip_marks(it, empty); //if none, returns 0
        pcount += skip_marks(it, partial);
        fcount += skip_marks(it, full);
        pcount += skip_marks(it, partial);
        ecount += skip_marks(it, empty);
    }
    
    if(it!=children.end()) //didn't go through all the children, therefore this node is in an irreducible form
    {
        fprintf(stderr, "PQnode::mark: irreducible node found\n");
        return false;
    }
    
    if(pcount>2) //no node subroot or not may ever have more than 2 partial chidren
    {
        return false;  
    }
    else if(pcount>0) //any number of partial nodes results in a partial parent node
    {
        node_mark = partial; 
    }
    else if(fcount>0&&ecount>0) //both full and empty nodes but no partials
    {
        node_mark = partial; 
    }
    else if(fcount>0) //ecount and pcount must be 0
    {
        node_mark = full; 
    }
    else //fcount and pcount must be 0
    {
        node_mark = empty; 
    }
    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: sort_children()
 * input: none
 * purpose:
 *      sorts the children of the node by marking. restricts the type of sort operations
 *      allowed to those type specific
 *      sorts to look e...p....f...
 * return: none
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQnode::sort_children()
{
    if(type==qnode)
    {
        std::list<Node*>::iterator it = children.begin();
        while( it!=children.end() )
        {
            size_t ecount = skip_marks(it, empty);
            skip_marks(it, partial);
            size_t fcount = skip_marks(it, full);
            size_t p2count = skip_marks(it, partial);
            size_t e2count = skip_marks(it, empty);
            
            if(ecount>0) //starts with an empty node
            {
                return;
            }
            else if(e2count>0) //ends with an empty node
            {
                children.reverse();
            }
            else if(fcount>0&&p2count>0) //starts with a partial node
            {
                children.reverse();
            }
        }
    }
    else
    {
        children.sort(compare_marking);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: unmark()
 * input: none
 * purpose:
 *      erases/reset the marked pertinent subtree. recurse down the tree from the
 *      pertinent subroot until you hit an empty node or a leaf
 * return: none
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQnode::unmark()
{ 
    //mark the node as empty
    if(node_mark==empty)
    {
        return;
    }
    node_mark = empty;
    //recurse by children, if they are not empty
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it)
    {
        (*it)->unmark();
    }
}

std::string PQnode::print_expression(print_option m/*none*/)
{
    std::string result = "";
    if(m==mark_option)
    {
        switch(node_mark)
        {
            case full:
                result += "f:";
                break;
            case empty:
                result += "e:";
                break;
            case partial:
                result += "p:";
                break;
        }
    }
    else if(m==depth_option)
    {
        result += std::to_string(depth)+": ";
    }
    if(type==qnode)
    {
        result += "[";
    }
    else
    {
        result += "{";
    }
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it)
    {
        result += " ";
        result += (*it)->print_expression(m);
        result += " ";
    }
    if(type==qnode)
    {
        result += "]";
    }
    else
    {
        result += "}";
    }
    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: reduce()
 * input: none
 * purpose:
 *      starts the reduction process based on the type of the pertinent subroot
 * return:
 *      true: reduction worked.
 *      false: reduction failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::reduce()
{    
    if(type==pnode)
    {
        return reduce_proot();
    }
    else
    {
        return reduce_qroot();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: reduce_proot()
 * input: none
 * purpose:
 *      checks that the node has 2 or less partial nodes
 *      applies the reduction rules
 * return:
 *      true: reduction worked.
 *      false: reduction failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::reduce_proot()
{
    if(follow){ printf("PQnode::reduce_proot()\n"); }
    
    std::list<Node*> empty_list;
    std::list<Node*> partials_list; //can only ever have two elements, already resticed from marking
    std::list<Node*> sec_partials_list;
    std::list<Node*> full_list; //temporary list to store directed node stuff
    
    sort_children();
    std::list<Node*>::iterator it=children.begin();
    
    size_t ecount = grab_marks(it, empty, empty_list);
    
    if(it!=children.end()) //deal with the first partial child
    { 
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial) //add all the children of the node lower than this one
        {
            if(ch->reduce(true)) // RECURSION
            {
                ch->pop_children(partials_list);
            }
            else
            {
                return false;
            }
            ++it;
        } 
    }
    
    //if there is another partial node. deal with it here but pass the opposite direction
    if(it!=children.end()) //find all the partial children
    { 
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial) //add all the children of the node lower than this one
        {
            if(ch->reduce(false)) //important! this makes the node group to the left
            { 
                ch->pop_children(sec_partials_list);
            }
            else
            {
                return false;
            }
            ++it;
        }
    }
    
    grab_marks(it, full, full_list);  
    
    children.clear();
    children.splice(children.end(), empty_list);//add the empty nodes back (still have the same parent)
    
    if(partials_list.empty()) //no partial children
    { 
        link_child(group_children(full_list));
        return true;
    }
    
    if(ecount>0) //has empty children
    { 
        PQnode *qtemp = dynamic_cast<PQnode*>(group_children(partials_list)); //create the new qnode to house the full and partial children
        if(qtemp==NULL)
        {
            return false;
        }
        
        qtemp->link_child(group_children(full_list)); //group the full nodes into a qnode with the partials
        
        for(it=sec_partials_list.begin(); it!=sec_partials_list.end(); ++it) //if there is another partial child, add it here
        {
            qtemp->link_child((*it));
        }
        sec_partials_list.clear();
        
        qtemp->set_type(qnode);
        
        if(!qtemp->mark()) //need to mark the new node
        {
            return false;
        }
        
        link_child(qtemp); //link the qnode to the parent node
        
    }
    else //no empty children. make the subroot a qnode
    { 
        for(it=partials_list.begin(); it!=partials_list.end(); ++it)
        {
            link_child((*it));
        }
        partials_list.clear();
        
        link_child(group_children(full_list)); //group the full nodes into a qnode with the partials
        
        //if there is another partial child, add it here
        for(it=sec_partials_list.begin(); it!=sec_partials_list.end(); ++it)
        {
            link_child((*it));
        }
        sec_partials_list.clear();
        set_type(qnode);
    }
    
    update_depth();
    return true;
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: pop_children(std::list<Node*> &kids)
 * input:
 *      kids: the list we want to add the children to
 * purpose:
 *      removes all the children from the current node and places them in in the input list
 * return: none
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQnode::pop_children(std::list<Node*> &kids)
{
    Node *temp = pop_child();
    while(temp!=NULL)
    {
        kids.push_back(temp);
        temp = pop_child();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: reduce_proot()
 * input: none
 * purpose:
 *      checks that the node conforms to the style e* p' f* p' e*
 *      applies the reduction rules
 * return:
 *      true: reduction worked.
 *      false: reduction failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::reduce_qroot()
{
    if(follow){ printf("PQnode::reduce_qroot()\n"); }
    
    sort_children();
    
    std::list<Node*> partials_list;
    std::list<Node*>::iterator it=children.begin();
    skip_marks(it, empty);
    
    if(!promote_partial_children(it, true)) //first potential partial node
    {
        return false;
    }
    
    skip_marks(it, full); //skip the middle full nodes
    
    if(!promote_partial_children(it, false)) //next potential partial node
    {
        return false;
    }
    update_depth();
    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: promote_partial_children(std::list<Node*>::iterator &it, bool direction)
 * input:
 *      it: the current position in the list of children
 *      direction = false: group fulls nodes to the left
 * purpose:
 *      takes all the grandkids from the partial node. removes them from the child and
 *      inserts them in front of the old child. removes the child 
 * return:
 *      true: promotion was successfull
 *      false: promotion failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::promote_partial_children(std::list<Node*>::iterator &it, bool direction)
{
    std::list<Node*> partials_list;
    if(it!=children.end())
    {
        PQnode *curr = dynamic_cast<PQnode*>(*it);
        if(curr!=NULL&&curr->get_mark()==partial)
        {
            it = children.erase(it);
            
            if(curr->reduce(direction)) //recursively deal with partial node
            {
                Node *temp = curr->pop_child();
                while(temp!=NULL)
                {
                    children.insert(it, temp);
                    temp->set_parent(this);
                    temp = curr->pop_child();
                }
                update_depth();
            }else
            {
                return false;
            }
            
            delete curr;
        }
        else
        {
            ++it;
        }
    }
    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: reduce(bool direction)
 * input:
 *      direction = true: group full nodes to the right;
 *      direction = false: group fulls nodes to the left
 * purpose:
 *      sends to other reduce functions depending on the type of node
 * return:
 *      true: reduction worked.
 *      false: reduction failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::reduce(bool direction)
{    
    if(type==pnode) //if a p node. call ordering funciton
    { 
        return preduce(direction);
    }
    else //if qnode, don't. but can reverse to ensure empty children are at the head of the list
    { 
        return qreduce(direction);
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: preduce(bool direction)
 * input: the direction (true: right, false: left) we group the full nodes toward
 * purpose:
 *      checks that the node conforms to the style e* p' f*
 *      applies the reduction rules
 * return:
 *      true: reduction worked.
 *      false: reduction failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::preduce(bool direction)
{
    if(follow){ printf("PQnode::preduce(bool direction)\n"); }
    
    std::list<Node*> empty_list;
    std::list<Node*> partials_list;
    std::list<Node*> full_list; //temporary list to store directed node stuff
    size_t ecount;
    size_t fcount;
    
    sort_children();
    
    std::list<Node*>::iterator it=children.begin();
    ecount = grab_marks(it, empty, empty_list);
    
    if(it!=children.end()) //find the partial child
    { 
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial)
        {
            //recurse
            //add all the children of the node lower than this one
            if(ch->reduce(direction))
            {
                Node *temp = ch->pop_child();
                while(temp!=NULL)
                {
                    partials_list.push_back(temp);
                    temp = ch->pop_child();
                }
            }else
            {
                return false;
            }
            ++it;
        } 
    }
    
    fcount = grab_marks(it, full, full_list);
    
    if(it!=children.end())
    {
        return false;
    }
    
    children.clear(); //remove so we can add back in in the right order after the merging
    
    link_child(group_children(empty_list)); //now group the children from the empty list into a pnode if necessary (more than 2)
    
    if(!partials_list.empty())
    {
        //now deal with the partial node children
        for(it=partials_list.begin(); it!=partials_list.end(); ++it)
        {
            link_child(*it);
        }
        partials_list.clear();
    }
    
    link_child(group_children(full_list)); //now finally the full node children. again you need to group them
    set_type(qnode);
    
    if(!direction)
    {
        children.reverse();
    }
    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: qreduce(bool direction)
 * input: the direction (true: right, false: left) we group the full nodes toward
 * purpose:
 *      checks that the node conforms to the style e* p' f*
 *      applies the reduction rules
 * return:
 *      true: reduction worked.
 *      false: reduction failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::qreduce(bool direction)
{
    if(follow){ printf("PQnode::qreduce(bool direction)\n"); }

    std::list<Node*> empty_list;
    std::list<Node*> full_list; //temporary list to store directed node stuff
    size_t count;
    
    sort_children();
    
    std::list<Node*>::iterator it=children.begin();
    count = skip_marks(it, empty); //node should now be partial or full
    if(!promote_partial_children(it, direction))
    {
        return false;
    }
    count = skip_marks(it, full); //node should now be partial or full
    
    if(it!=children.end())
    {
        return false;
    }
    if(direction==false)
    {
        children.reverse();
    }
    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: group_children(std::list<Node*> group)
 * input: list of node we wish to group
 * purpose: groups all the elements in a given list into a single pnode and returns the node
 * return: the new pnode. returns NULL if the list is empty or the marking fails
 * notes: clears the input list
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Node* PQnode::group_children(std::list<Node*> &group)
{
    if(follow){ printf("PQnode::group_children(std::list<Node*> group)\n"); }
    
    Node *result = NULL;
    if(group.empty())
    {
        return result;
    }
    else if(group.size()==1)
    {
        result = group.front();
    }
    else
    {
        PQnode *temp = new PQnode();
        for(std::list<Node*>::iterator it=group.begin(); it!=group.end(); ++it)
        {
            temp->link_child(*it);
        }
        if(!temp->mark())
        {
            return NULL;
        }
        result = temp;
    }
    group.clear();
    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: link_child(Node *child)
 * input: the node we wish to make a child of the current node
 * purpose: adds the input node to the current node and updates the pertinent
 *      parameters, also updates the depth of the child and its descedants
 * return: false if the input it NULL, true if the linking succeeds
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::link_child(Node *child)
{
    if(follow){ printf("PQnode::link_child(Node *child)\n"); }
    
    if(child==NULL)
    {
        return false;
    }
    child->set_parent(this);
    children.push_back(child);
    update_depth();
    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: update_depth()
 * input: none
 * purpose: recursively goes down the tree updating the depth parameter from the
 *      current node
 * return: none
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQnode::update_depth()
{
    int d = 0;
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it)
    {
        if((*it)->get_depth()>d)
        {
            d = (*it)->get_depth();
        }
    }
    depth = d + 1;
    
    if(parent!=NULL)
    {
        parent->update_depth();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: pop_child()
 * input: none
 * purpose: removes and returns the first child
 * return: the first child, or NULL if the children list is empty
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Node* PQnode::pop_child()
{
    if(children.empty())
    {
        return NULL;
    }
    else
    {
        Node *temp = children.front();
        children.pop_front();
        temp->set_parent(NULL);
        return temp;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: count_children()
 * input: none
 * purpose: sets the type. if qnode, makes sure that it isa valid qnode. otherwise
 *      it defaults to setting it to a pnode
 * return: the number (size_t) of children in the node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
size_t PQnode::count_children()
{
    return children.size(); 
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: set_type(nodetype t)
 * input: the type we want to set
 * purpose: sets the type. if qnode, makes sure that it isa valid qnode. otherwise
 *      it defaults to setting it to a pnode
 * return: none
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PQnode::set_type(nodetype t)
{
    if(t==qnode&&count_children()>2)
    {
        type = qnode;
    }
    else
    {
        type = pnode;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: condense_and_replace(Node *child)
 * input: the child we want to replace the full leaves with
 * purpose: if the full leaves are consecutive, then they are removed and replaced
 *      with the input node
 * return: false: if an error occurs or the condesation is impossible; true if success
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool PQnode::condense_and_replace(Node *child)
{
    if(follow){ printf("PQnode::condense_and_replace(Node *child)\n"); }
    if(child==NULL)
    {
        fprintf(stderr, "ERROR condense_and_replace: cannot replace with a null\n");
        return false;
    }
    std::list<Node*>::iterator it=children.begin();
    while(it!=children.end())
    {
        if((*it)->get_mark()==full)
        {
            if(dynamic_cast<Leaf*>(*it))
            {
                break;
            }
            else
            {
                fprintf(stderr, "ERROR: condense_leaves() - this node has full children that are not leaves\n");
                return false;
            }
        }
        ++it;
    }
       
    while(it!=children.end()) //found the first value. now delete all those after it with the same value 
    {
        if((*it)->get_mark()==full)
        {
            if(Leaf* lf = dynamic_cast<Leaf*>(*it))
            {
                it = children.erase(it);
                delete lf;
            }
            else
            {
                fprintf(stderr, "ERROR: condense_leaves() - this node has full children that are not leaves\n");
                return false;
            }
        }
        else
        {
            ++it;
        }
    }
    //now the iterator is at the item just after the last full leaf
    children.insert(it, child);
    child->set_parent(this);
    update_depth();
    
    return true;
}

//test if another node is equivalent currently this method is O(n2) where n is the number of children for pnodes
bool PQnode::is_equivalent(Node *other)
{
    //std::stack<char> node_structure; //stack for computing bracket closure etc
    std::set<int> leaves; //set of the leaf values in this node
    std::list<PQnode*> pnodes; //list of all the pnodes in the current node
    std::list<PQnode*> qnodes; //list of all the qnodes in the current node
    std::list<PQnode*> pnodes_other;
    std::list<PQnode*> qnodes_other;
    
    if( PQnode *pq = dynamic_cast<PQnode*>(other) ) //make sure it is the same type
    {
        if( count_children()!=pq->count_children() ) //makes sure they have the same number of children
        {
            return false;
        }
        
        
        
    }
    return false;
}

std::list<Node*>* PQnode::get_children()
{
    return &children;
}

void PQnode::reverse_children()
{
    children.reverse();
}

