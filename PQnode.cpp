

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Class PQNode: public Node
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "PQnode.h"

PQnode::PQnode(){
    type = pnode;
    parent = NULL;
    depth = 0;
}

PQnode::~PQnode(){
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        delete *it;
    }
    children.clear();
}

/* every new node should start with a set of leaves.
 * should only link PQ nodes by replacing leaves in already existing nodes */
PQnode::PQnode(std::vector<int> leaves, nodetype t/* = pnode*/){
    parent = NULL;
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
        if(Leaf *lf = dynamic_cast<Leaf*>((*it))){ //if the child is a leaf
            leaves.push_back(lf);
            lf->set_leaf_list_ptr(&leaves.back());
            std::cout << "leaf " << lf << " points to " << lf->get_leaf_list_ptr() << "\n" << std::endl;
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
    std::cout << "remove_child() " << std::endl;
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(Leaf* lf = dynamic_cast<Leaf*>(*it)){
            if(lf->get_value()==value){
                std::cout << "found the node with the value " << value << std::endl;
                children.remove(*it);
                std::cout << "about to call destructor " << std::endl;
                delete lf; //this will call lf's destructor
                std::cout << "after the destructor " << std::endl;
                return true;
            }
        }
    }
    return false;
}
