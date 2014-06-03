

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class PQNode: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "PQnode.h"

static bool test_leaks = false;

PQnode::PQnode(): Node(){
    if(test_leaks){ printf("PQNODE ++\n");}
    type = pnode;
}

PQnode::~PQnode(){
    if(test_leaks){ printf("PQNODE --\n");}
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        delete *it;
    }
    children.clear();
}

/* every new node should start with a set of leaves.should only link PQ nodes by replacing leaves in already existing nodes 
PQnode::PQnode(std::vector<int> leaves, nodetype t){ //t default is pnode
    if(test_leaks){ printf("PQNODE ++\n");}
    parent = NULL;
    depth = 0;
    for(size_t i=0; i<leaves.size(); i++){
        Leaf *temp = new Leaf(this, leaves[i]); // create the new leaf
        children.push_back(temp); // add the new leaf to the set of children
    }
    type = (children.size()<3)? pnode: t; //if there are less than 3 children this is a pnode even if you try to declare a qnode
}

PQnode::PQnode(Node *p, std::vector<int> leaves, nodetype t){ // nodetype t= pnode
    if(test_leaks){ printf("PQNODE ++\n");}
    parent = p;
    depth = 0;
    for(size_t i=0; i<leaves.size(); i++){
        Leaf *temp = new Leaf(this, leaves[i]); // create the new leaf
        children.push_back(temp); // add the new leaf to the set of children
    }
    type = (children.size()<3)? pnode: t; //if there are less than 3 children this is a pnode even if you try to declare a qnode
}*/

void PQnode::print(){
    printf("+++++++++++++ node-type: %s  +++++++++++\n", (type==pnode)? "P": "Q");
    Node::print();
    printf("num children: %lu ... \n\n", children.size());
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        (*it)->print();
    }
    
}

/*
void PQnode::add_leaves(std::list<Leaf*> &leaves){
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(Leaf *lf = dynamic_cast<Leaf*>(*it)){ //if the child is a leaf
            leaves.push_back(lf);
            lf->set_leaf_list_ptr(&leaves.back());
        }
    }
}*/

//check the children in order to mark the node
int PQnode::mark_node(){
    int fcount = 0;
    int pcount = 0;
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        switch((*it)->get_mark()){
            case full:
                fcount++;
                break;
            case partial:
                pcount++;
                if(pcount>2){
                    return -1;
                }
                break;
            default: //if unlabelled, assume empty
                break;
        }
    }
    if(pcount>0){
        node_mark = partial;
    }else if(fcount>0){
        if(fcount<children.size()){
            node_mark = partial;
        }else{
            node_mark = full;
        }
    }else{
        node_mark = empty;
    }
    return 0;
}

bool PQnode::remove_child(int value){
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(Leaf* lf = dynamic_cast<Leaf*>(*it)){
            if(lf->get_value()==value){
                //children.remove(*it);
                children.erase(it);
                delete lf; //this will call lf's destructor
                return true;
            }
        }
    }
    return false;
}


bool PQnode::replace_child(int value, Node *child){
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(Leaf* lf = dynamic_cast<Leaf*>(*it)){
            if(lf->get_value()==value){
                children.insert(it, child);
                child->set_parent(this);
                children.erase(it);
                delete lf; //this will call lf's destructor
                child->update_depth();
                return true;
            }
        }
    }
    return false;
}


void PQnode::sort_children(){
    children.sort(compare_marking);
}


/*******************************************************************************
 * Function PQnode::unmark(PQnode* subroot)
 * purpose: erases/reset the marked pertinent subtree
 * recurse down the tree from the pertinent subroot until you hit an empty node or a leaf
 ********************************************************************************/
void PQnode::unmark(){ //passes by ref
    //mark the node as empty
    if(node_mark==empty){
        return;
    }
    node_mark = empty;
    //recurse by children, if they are not empty
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(PQnode *p = dynamic_cast<PQnode*>(*it)){
            p->unmark();
        }else{
            (*it)->unmark();
        }
    }
}

void PQnode::print_expression(bool m/*false*/){
    if(m){
        switch(node_mark){
            case full:
                printf("f:");
                break;
            case empty:
                printf("e:");
                break;
            case partial:
                printf("p:");
                break;
        }
    }
    if(type==qnode){
        printf("[");
    }else{
        printf("{");
    }
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        printf(" ");
        (*it)->print_expression(m);
        printf(" ");
    }
    if(type==qnode){
        printf("]");
    }else{
        printf("}");
    }
}


bool PQnode::reduce(){
    if(type==pnode){
        return reduce_proot();
    }else{
        //reduce_qroot();
        return false;
    }
    update_depth();   
}

bool PQnode::reduce_proot(){
    
    std::list<Node*> empty_list;
    std::list<Node*> partials_list;
    std::list<Node*> sec_partials_list;
    std::list<Node*> full_list; //temporary list to store directed node stuff
    
    sort_children();
    std::list<Node*>::iterator it=children.begin();
    while(it!=children.end()){ //find all the empty children
        if((*it)->get_mark()==empty){
            empty_list.push_back((*it));
        }else{
            break;
        }
        ++it;
    }
    
    //deal with the first partial child
    if(it!=children.end()){ //find all the partial children
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial){
            //add all the children of the node lower than this one
            if(ch->reduce(true)){
                Node *temp = ch->pop_child();
                while(temp!=NULL){
                    partials_list.push_back(temp);
                    temp = ch->pop_child();
                }
            }else{
                return false;
            }
            ++it;
        } 
    }
    
    //if there is another partial node. deal with it here but pass the opposite direction
    if(it!=children.end()){ //find all the partial children
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial){
            //add all the children of the node lower than this one
            if(ch->reduce(false)){ //important! this makes the node group to the left
                Node *temp = ch->pop_child();
                while(temp!=NULL){
                    sec_partials_list.push_back(temp);
                    temp = ch->pop_child();
                }
            }else{
                return false;
            }
            ++it;
        }
    }
    
    //deal with the full nodes
    while(it!=children.end()){ //find all the full children
        if((*it)->get_mark()==full){
            full_list.push_back((*it));
        }else{
            return false;
        }
        ++it;
    }    
    
    
    children.clear();
    children.splice(children.end(), empty_list);//add the empty nodes back (still have the same parent)
    
    if(partials_list.empty()){ //no partial children
        link_child(group_children(full_list));
        return true;
    }
    //create the new qnode to house the full and partial children
    PQnode *qtemp = new PQnode();
    
    for(it=partials_list.begin(); it!=partials_list.end(); ++it){
        qtemp->link_child((*it));
    }
    partials_list.clear();
    
    //group the full nodes into a qnode with the partials
    qtemp->link_child(group_children(full_list));
    
    //if there is another partial child, add it here
    for(it=sec_partials_list.begin(); it!=sec_partials_list.end(); ++it){
        qtemp->link_child((*it));
    }
    sec_partials_list.clear();
    
    qtemp->set_type(qnode);
    
    //link the qnode to the parent node
    qtemp->mark_node();
    link_child(qtemp);
    
    return true;
    
}

//check if the tree is reducible during the marking phase. will save a lot of headache
bool PQnode::reduce_qroot(){
    //should be of the form e* p f* p e*
    std::list<Node*> partials_list;
    std::list<Node*>::iterator it=children.begin();
    while(it!=children.end()){ //skip the empty nodes
        if((*it)->get_mark()!=empty){
            break;
        }else{
            ++it;
        }
    }
    //first potential partial node
    if(it!=children.end()){
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial){
            //recursively deal with partial node
            //add all the children of the node lower than this one
            if(ch->reduce(true)){
                Node *temp = ch->pop_child();
                while(temp!=NULL){
                    partials_list.push_back(temp);
                    temp = ch->pop_child();
                }
            }else{
                return false;
            }
            children.erase(it);
            children.insert(it, group_children(partials_list));
            delete ch;
            partials_list.clear();
            ++it;
        }
    }
    //skip the middle full nodes
    while(it!=children.end()){
        if((*it)->get_mark()!=empty){
            break;
        }else{
            ++it;
        }
    }
    
    //next potential partial node
    if(it!=children.end()){
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial){
            if(ch->reduce(false)){
                Node *temp = ch->pop_child();
                while(temp!=NULL){
                    partials_list.push_back(temp);
                    temp = ch->pop_child();
                }
            }else{
                return false;
            }
            children.erase(it);
            children.insert(it, group_children(partials_list));
            delete ch;
            partials_list.clear();
        }
    }
    return true;
}

//node direction is true = right, left = false
//for a partial node and returns it's children as a list
//if this is not a partial node. returns an error
//will not change the leaflist at all
//cannot use on the subroot
//returns an error id any is unreducible
bool PQnode::reduce(bool direction){
    printf("before reducing the current node: ");
    print_expression(true);
    printf("\n");
    //create the lists
    std::list<Node*> empty_list;
    std::list<Node*> partials_list;
    std::list<Node*> full_list; //temporary list to store directed node stuff
    
    if(type==pnode){ //if a p node. call ordering funciton
        sort_children();
    }else{ //if qnode, don't. but can reverse to ensure empty children are at the head of the list
        if(!children.empty()){
            if(children.front()->get_mark()!=empty){
                children.reverse();
            }
        }
    }
    
    std::list<Node*>::iterator it=children.begin();
    //it!=children.end(); ++it;
    while(it!=children.end()){ //find all the empty children
        if((*it)->get_mark()==empty){
            empty_list.push_back((*it));
        }else{
            break;
        }
        ++it;
    }
    
    if(it!=children.end()){ //find all the partial children
        PQnode *ch = dynamic_cast<PQnode*>(*it);
        if(ch&&(*it)->get_mark()==partial){
            //recurse
            //add all the children of the node lower than this one
            if(ch->reduce(direction)){
                Node *temp = ch->pop_child();
                while(temp!=NULL){
                    partials_list.push_back(temp);
                    temp = ch->pop_child();
                }
            }else{
                return false;
            }
            ++it;
        } 
    }
    
    while(it!=children.end()){ //find all the full children
        if((*it)->get_mark()==full){
            full_list.push_back((*it));
        }else{
            return false;
        }
        ++it;
    }
        
    children.clear(); //remove so we can add back in in the right order after the merging
    
    //now group the children from the empty list into a pnode if necessary (more than 2)
    link_child(group_children(empty_list));
    
    if(!partials_list.empty()){
        //now deal with the partial node children
        for(it=partials_list.begin(); it!=partials_list.end(); ++it){
            link_child(*it);
        }
        partials_list.clear();
    }
    
    //now finally the full node children. again you need to group them
    link_child(group_children(full_list));
    
    set_type(qnode);
    
    return true;
}

//groups all the elements in a given list into a single pnode and returns the node
//also clears the list
Node* PQnode::group_children(std::list<Node*> group){
    Node *result = NULL;
    if(group.empty()){
        return result;
    }else if(group.size()==1){
        result = group.front();
    }else{
        PQnode *temp = new PQnode();
        for(std::list<Node*>::iterator it=group.begin(); it!=group.end(); ++it){
            temp->link_child(*it);
        }
        temp->mark_node();
        result = temp;
    }
    group.clear();
    return result;
}

//linking a new child should only be done if said child is already part of the tree.
//otherwise you need to update the leaf list
//returns false if an error occurred. i.e. you try adding a node without a parent.
//i.e. not part of the tree
bool PQnode::link_child(Node *child){
    //need to link the new child
    if(child==NULL){
        return false;
    }
    child->set_parent(this);
    children.push_back(child);
    child->update_depth();
    return true;
}

//recursive goes down the tree updating the depth parameter from the current node
void PQnode::update_depth(){
    if(parent==NULL){
        depth = 0;
    }else{
        depth = parent->get_depth() + 1;
        for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
            (*it)->update_depth();
        }
    }
}

//removes and returns the first child from a given node
//returns null when there are no children left
Node* PQnode::pop_child(){
    if(children.empty()){
        return NULL;
    }else{
        Node *temp = children.front();
        children.pop_front();
        temp->set_parent(NULL);
        return temp;
    }
}

int PQnode::count_children(){
    return children.size(); 
}

void PQnode::set_type(nodetype t){
    if(t==qnode&&count_children()>2){
        type = qnode;
    }else{
        type = pnode;
    }
}
