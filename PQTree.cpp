
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

PQTree::PQTree(){
    root = NULL;
}

PQTree::PQTree(std::vector<int> leaves){
    root = new PQnode(leaves);
}

PQTree::PQTree(std::string const expr){
    size_t i=0;
    if(PQnode *tmp = dynamic_cast<PQnode*>(build_from_expr(expr, i))){
        root = tmp;
    }else{
        root = NULL;
        fprintf(stderr, "cannot initialize a pq tree with an invalid expression\n");
        exit(1);
    }
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

//use for planatiry testing
//input vector of ints represents the leaves of the new universal tree
bool PQTree::reduce_and_replace(int v, std::vector<int> tree_in){
    if(follow){ printf("PQTree::reduce_on(int value, std::vector<int> tree_in)\n"); }
    
    //put the value inside a vector
    std::vector<int> values;
    values.push_back(v);
    
    PQnode* subroot = reduce(values); //pertinent subroot
    if(subroot==NULL){ return false; }
    
    Node *child;
    if(tree_in.size()==1){
        child = new Leaf(tree_in[0]);
    }else{
        child = new PQnode(tree_in);
    }
    if(!replace_full_with(child)){ return false; }
    
    subroot->unmark();
    
    return true;
}

PQnode* PQTree::reduce(std::vector<int> values){
    if(follow){ printf("PQTree::reduce(int value)\n"); }
    
    PQnode* subroot = mark(values); //pertinent subroot
    print_expression(true);
    if(follow){ printf("PQTree::reduce(int value). marked as "); print_expression(true); }
    if(subroot!=NULL){
        if(subroot->reduce()){
            return subroot;
        }
    }
    return NULL;
}

bool PQTree::set_consecutive(std::vector<int> values){
    PQnode *subroot = reduce(values);
    if(subroot==NULL){ return false; }
    
    subroot->unmark();
    return true;
}

bool PQTree::replace_full_with(Node *child){
    if(follow){ printf("PQTree::replace_full_with(Node *child)\n"); }
    
    Node* parent = NULL;
    if(child==NULL){ return false; }
    std::list<Leaf*> fulls = Leaf::get_pertinent();
    
    for(std::list<Leaf*>::iterator it=fulls.begin(); it!=fulls.end(); ++it){
        if(parent){
            if((*it)->get_parent()!=parent){
                fprintf(stderr, "not all full nodes have the same parent\n");
                return false;
            }
        }else{
            parent = (*it)->get_parent();
        }
    }
    
    if(PQnode* temp = dynamic_cast<PQnode*>(parent)){
        if(!(temp->condense_and_replace(child))){
            fprintf(stderr, "condense failed\n");
            return false;
        }
        if(temp->count_children()<3){
            temp->set_type(pnode);
        }
        
    }
    return true;
}

/*******************************************************************************
 * Function PQTree::mark(int value)
 * purpose: marks the pertinent subtree
 * returns the subroot of the pertinent subtree, otherwise NULL if an error occurs
 ********************************************************************************/
PQnode* PQTree::mark(std::vector<int> v){
    
    if(follow){ printf("PQTree::mark(int v)\n"); }
    
    std::list<Leaf*> fulls = Leaf::mark_pertinent(v); //mark the full leaves based on the input values
        
    //now find the parents of all the full leaves. add to the partials list but do not add duplicates
    std::list<PQnode*> partials;
    for(std::list<Leaf*>::iterator k = fulls.begin(); k!=fulls.end(); ++k){
        PQnode *p = dynamic_cast<PQnode*>((*k)->get_parent());
        if(p==NULL){ return NULL;} //this is the parent we want to add to the list of potential partials
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
        
        PQnode *curr = partials.front();//always deal with the front element first
        curr->mark_node(); //mark the node
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
    return NULL;
    
}

//prints an expression that represents the current tree. {} are p nodes, [] are qnodes
void PQTree::print_expression(bool mark/*false*/){
    root->print_expression(mark);
    printf("\n");
}


//takes in a string expression of a pq-tree and builds the corresponding tree
Node* PQTree::build_from_expr(std::string const expr, size_t &i){
    if(follow){ printf("PQTree::build_from_expr(std::string const expr, int &i)\n"); }
    
    int state = 0;
    bool reading = true;
    bool isqnode = false;
    PQnode *rt = NULL;
    Leaf *lf = NULL;
    
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
                    Node *child = build_from_expr(expr, i);
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
                    lf = new Leaf(rt, atoi(num.c_str()));
                    rt->link_child(lf);
                }
                break;
            default:
                return NULL;
        }
    }
    if(isqnode&&rt->count_children()>2){
        rt->set_type(qnode);
    }else if(rt->count_children()==1){
        return lf;
    }
    return rt;
}














