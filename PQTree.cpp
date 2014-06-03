
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


PQTree::PQTree(){
    root = NULL;
}

PQTree::PQTree(std::string const expr){
    int i=0;
    root = build_from_expr(expr, i);
}

PQTree::~PQTree(){
    delete root;
    root = NULL;
}

//iterates recusively through the tree and prints out leaves and nodes
void PQTree::print(){
    if(root==NULL){
        printf("Empty tree\n");
    }else{
        root->print();
    }
}

/* ***************************************************************************************
 * procedure: reduce_on(int value)
 * purpose: marks the nodes as full or empty based on the given value
 * then applies the templates to enforce consecutive ordering of the nodes with this value
 * returns -1 if the reduciton step fails
 * returns 0 if the reduction step is successfull.
 *****************************************************************************************/
int PQTree::reduce_on(int value, std::vector<int> v){
    //print();
    /*
    PQnode* subroot = mark(value); //pertinent subroot
    print_expression(true);
    subroot->reduce();
    PQnode *temp = new PQnode(v);
    temp->add_leaves(leaflist);
    //current just replace 2
    if(!subroot->replace_child(2, temp)){
        return -1;
    }
    //now reduce on 3
    print_expression();
    printf("re-mark with the value 3\n");
    subroot->unmark();
    printf("...\n");
    subroot = mark(3);
    //print();
    print_expression(true);
    if(!subroot){ printf("error in marking the subroot\n"); return -1; }
    subroot->reduce();
    printf("reduced tree.... \n");
    print_expression(true);
    //now unmark the tree. recurse down the tree from the pertinent subroot until you hit an empty node or a leaf
    //subroot->unmark();*/
    return 0;
}

/*******************************************************************************
 * Function PQTree::mark(int value)
 * purpose: marks the pertinent subtree
 * returns the subroot of the pertinent subtree, otherwise NULL if an error occurs
 ********************************************************************************/
PQnode* PQTree::mark(int v){
    printf("before marking ");
    print_expression(true);
    std::vector<Leaf*> fulls;
    clean_leaflist();
    //mark the full leaves based on the input value
    for(std::list<Leaf*>::iterator it=leaflist.begin(); it!=leaflist.end(); ++it){
        if((*it)->get_value()==v){
            (*it)->mark();
            fulls.push_back((*it));
        }
    }
    
    //now find the parents of all the full leaves. add to the partials list but do not add duplicates
    std::list<PQnode*> partials;
    for(size_t i=0; i<fulls.size(); ++i){
        PQnode *p = (PQnode*)fulls[i]->get_parent(); //this is the parent we want to add to the list of potential partials
        //add it into the partials list by inserting it at the correct position based on decreasing depth
        if(partials.empty()){
            partials.push_back(p);
        }else{
            for(std::list<PQnode*>::iterator it=partials.begin(); it!=partials.end(); ++it){ //iterates through our list of partials
                if((*it)->get_depth()>=p->get_depth()){
                    if(p==(*it)){
                        break;
                    }
                }else{
                    partials.insert(it, p);
                }
            }
        }
    }
    
    //at this point we have a list of potential partials sorted by depth with no duplicates
    //now we need to mark these nodes.... and then their parents until there is only one node left in the partials list
    while(partials.size()>1){
        //always deal with the front element first
        //mark the node
        PQnode *curr = partials.front();
        curr->mark_node();
        PQnode *p = (PQnode*)curr->get_parent(); //any parent in the tree will never be a leaf since they cannot have children. therefore this casting is safe
        partials.pop_front(); //remove the curr node and destroy the reference
        
        //add in the appropriate place. 
        for(std::list<PQnode*>::iterator it=partials.begin(); it!=partials.end(); ++it){ //iterates through our list of partials
            if((*it)->get_depth()>=p->get_depth()){
                if(p==(*it)){
                    break;
                }
            }else{
                partials.insert(it, p);
            }
        }
    }
    if(!partials.empty()){
        partials.front()->mark_node();
        return partials.front();
    }
    printf("after marking ");
    print_expression(true);
    return NULL;
    
}

void PQTree::print_leaflist(bool detail/*false*/){
    if(detail){
        printf("printing the leaflist .........\n");
        for(std::list<Leaf*>::iterator it=leaflist.begin(); it!=leaflist.end(); ++it){
            if((*it)!=NULL){
                (*it)->print();
            }else{
                printf("NULL\n");
            }
        }
        return;
    }
    
    for(std::list<Leaf*>::iterator it=leaflist.begin(); it!=leaflist.end(); ++it){
        if((*it)!=NULL){
            printf("%d ", (*it)->get_value());
        }else{
            printf("NULL ");
        }
    }
    printf("\n");
    
}

//prints an expression that represents the current tree. {} are p nodes, [] are qnodes
void PQTree::print_expression(bool mark/*false*/){
    root->print_expression(mark);
    printf("\n");
}

void PQTree::clean_leaflist(){
    leaflist.remove(NULL);
}

//takes in a string expression of a pq-tree and builds the corresponding tree
PQnode* PQTree::build_from_expr(std::string const expr, int &i){
    int state = 0;
    bool reading = true;
    bool isqnode = false;
    PQnode *rt = NULL;
    
    while(reading&&i<expr.length()){
        switch(state){
            case 0:
                if(isspace(expr[i])){ //ignore whitespace
                    ++i;
                }else if(expr[i]=='{'||expr[i]=='['){
                    if(expr[i]=='['){
                        isqnode = true;
                    }
                    state = 1; ++i; rt = new PQnode();
                }else{
                    return NULL;
                }
                break;
            case 1: //started a node. linking children
                if(isspace(expr[i])){ //ignore whitespace
                    ++i;
                }else if(expr[i]=='{'||expr[i]=='['){ //start the next pnode
                    PQnode *child = build_from_expr(expr, i);
                    rt->link_child(child);
                    ++i;
                }else if(expr[i]=='}'||expr[i]==']'){
                    reading = false;
                }else if(isdigit(expr[i])){
                    //get the number terminated by a comma
                    std::string num = "";
                    while(expr[i]!=' '&&expr[i]!=']'&&expr[i]!='}'&&i<expr.length()){
                        num += expr[i++];
                    }
                    Leaf *lf = new Leaf(rt, atoi(num.c_str()), leaflist);
                    rt->link_child(lf);
                }
                break;
            default:
                return NULL;
                break;
        }
    }
    if(isqnode){
        rt->set_type(qnode);
    }
    return rt;
}














