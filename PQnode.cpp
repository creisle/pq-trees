

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class PQNode: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "PQnode.h"

PQnode::PQnode(){
    printf("PQNODE CONTRUCTOR ++\n");
    type = pnode;
    parent = NULL;
    depth = 0;
}

PQnode::~PQnode(){
    printf("PQNODE DESTRUCTOR --\n");
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        delete *it;
    }
    children.clear();
}

/* every new node should start with a set of leaves.
 * should only link PQ nodes by replacing leaves in already existing nodes */
PQnode::PQnode(std::vector<int> leaves, nodetype t/* = pnode*/){
    printf("PQNODE CONTRUCTOR ++\n");
    parent = NULL;
    depth = 0;
    for(size_t i=0; i<leaves.size(); i++){
        Leaf *temp = new Leaf(this, leaves[i]); // create the new leaf
        children.push_back(temp); // add the new leaf to the set of children
    }
    type = (children.size()<3)? pnode: t; //if there are less than 3 children this is a pnode even if you try to declare a qnode
}

/* every new node should start with a set of leaves.
 * should only link PQ nodes by replacing leaves in already existing nodes */
PQnode::PQnode(Node *p, std::vector<int> leaves, nodetype t/* = pnode*/){
    printf("PQNODE CONTRUCTOR ++\n");
    parent = p;
    depth = 0;
    for(size_t i=0; i<leaves.size(); i++){
        Leaf *temp = new Leaf(this, leaves[i]); // create the new leaf
        children.push_back(temp); // add the new leaf to the set of children
    }
    type = (children.size()<3)? pnode: t; //if there are less than 3 children this is a pnode even if you try to declare a qnode
}

void PQnode::print(){
    printf("+++++++++++++ node-type: %s  +++++++++++\n", (type==pnode)? "P": "Q");
    Node::print();
    printf("num children: %lu ... \n\n", children.size());
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        (*it)->print();
    }
    
}

void PQnode::add_leaves(std::list<Leaf*> &leaves){
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(Leaf *lf = dynamic_cast<Leaf*>(*it)){ //if the child is a leaf
            leaves.push_back(lf);
            lf->set_leaf_list_ptr(&leaves.back());
        }
    }
}

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
        mark = partial;
    }else if(fcount>0){
        if(fcount<children.size()){
            mark = partial;
        }else{
            mark = full;
        }
    }else{
        mark = empty;
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
                return true;
            }
        }
    }
    return false;
}


/*********************************************************************************************************************
 * function reducable()
 * purpose: test if the current node is in a reducble form. if it is, then this performs the appropriate reduction
 * note: on an unmarked tree, all nodes are assumed to be empty.
 *********************************************************************************************************************/

//will need to impose further restrictions if it is reducible
//organize into e*...p...f*...p
//means that we just have to move the last p to the end
//turn this into a q-node
//group all the e and f into p nodes....
//f and p are restricted to being next to one another. but emptys can go  whereever. so don't make the parent a q node

bool PQnode::subroot_p_reducible(){
    sort_children();
    int part = 0;
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if((*it)->get_mark()==partial){
            part++;
            if(part>2){
                return false;
            }
        }
    }
    return true;
}

bool PQnode::subroot_q_reducible(){
    std::list<Node*>::iterator it=children.begin();
    int state = 0;
    while(it!=children.end()){
        switch(state){
            case 0: //this is the default starting case
                if((*it)->get_mark()==full){
                    state = 2; ++it; 
                }else if((*it)->get_mark()==partial){
                    state = 3; ++it;
                }else{ //assume empty
                    state = 4; ++it;
                }
                break;
            case 1: // f* ...
                if((*it)->get_mark()==full){
                    ++it; 
                }else if((*it)->get_mark()==partial){
                    state = 4; ++it;
                }else{ //assume empty
                    state = 4; ++it;
                }
                break;
            case 2: // p ...
                if((*it)->get_mark()==full){
                    state = 1; ++it; 
                }else if((*it)->get_mark()==partial){
                    state = 4; ++it;
                }else{ //assume empty
                    state = 4; ++it;
                }
                break;
            case 3: // e* ...
                if((*it)->get_mark()==full){
                    state = 1; ++it; 
                }else if((*it)->get_mark()==partial){
                    state = 2; ++it;
                }else{ //assume empty
                    ++it;
                }
                break;
            case 4: // ... e*
                if((*it)->get_mark()==full){
                    return false; 
                }else if((*it)->get_mark()==partial){
                    return false;
                }else{ //assume empty
                    ++it;
                }
                break;
            default: // if we end up here then an error occured, return false
                return false;
                break;
        }
    }
    return true;
}

/***********************************************************************************************
 * function PQnode::q_reducible()
 * purpose: test a non-subroot node to see if it matching any of the allowed template structures
 * templates allowed: {e* p f*} or {f* p e*}
 * returns true if the node matches a valid template. false otherwise
 **********************************************************************************************/
bool PQnode::q_reducible(){
    std::list<Node*>::iterator it=children.begin();
    int state = 0;
    while(it!=children.end()){
        switch(state){
            case 0: //this is the default starting case
                if((*it)->get_mark()==full){
                    state = 1; ++it; 
                }else if((*it)->get_mark()==partial){
                    state = 2; ++it;
                }else{ //assume empty
                    state = 3; ++it;
                }
                break;
            case 1: // f...
                if((*it)->get_mark()==full){
                    ++it; 
                }else if((*it)->get_mark()==partial){
                    state = 4; ++it;
                }else{ //assume empty
                    state = 5; ++it;
                }
                break;
            case 2: // p...
                if((*it)->get_mark()==full){
                    state = 6; ++it; 
                }else if((*it)->get_mark()==partial){
                    return false;
                }else{ //assume empty
                    state = 5; ++it;
                }
                break;
            case 3: // e...
                if((*it)->get_mark()==full){
                    state = 6; ++it; 
                }else if((*it)->get_mark()==partial){
                    state = 7; ++it;
                }else{ //assume empty
                    ++it;
                }
                break;
            case 4: //looking for e
                if((*it)->get_mark()==empty){
                    state = 5; ++it; 
                }else{ 
                    return false;
                }
                break;
            case 5: //looking for ... e*
                if((*it)->get_mark()==empty){
                    ++it; 
                }else{
                    return false;
                }
                break;
            case 6: //looking for ... f*
                if((*it)->get_mark()==full){
                    ++it;
                }else{ //assume empty
                    return false;
                }
                break;
            case 7: //looking for ... f
                if((*it)->get_mark()==full){
                    state = 6; ++it;
                }else{ //assume empty
                    return false;
                }
                break;
            default: // if we end up here then an error occured, return false
                return false;
                break;
        }
    }
    return true;
}

/***********************************************************************************************
 * function PQnode::q_reducible()
 * purpose: test a non-subroot node to see if it matching any of the allowed template structures
 * also sort the p-node
 * returns true if the node matches a valid template. false otherwise
 **********************************************************************************************/
bool PQnode::p_reducible(){
    //sort the p-node into empty...partial...full
    sort_children();
    bool part = false;
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if((*it)->get_mark()==partial){
            if(part){
                return false;
            }
            part = true;
        }
    }
    return true;
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
    mark = empty;
    //recurse by children, if they are not empty
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(PQnode *p = dynamic_cast<PQnode*>(*it)){
            p->unmark();
        }else{
            (*it)->unmark();
        }
    }
}

void PQnode::print_expression(){
    if(type==qnode){
        printf("[");
    }else{
        printf("{");
    }
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        printf(" ");
        (*it)->print_expression();
        printf(" ");
    }
    if(type==qnode){
        printf("]");
    }else{
        printf("}");
    }
}






