
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

// non-default constructor will build a universal tree,
// i.e. p-node from a given set of input values
PQTree::PQTree(std::vector<int> set){
    root = new PQnode(set);
    root->add_leaves(leaflist);//add the new leaves to the leaflist...
    
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
    PQnode* subroot = mark(value); //pertinent subroot
    PQnode *temp = new PQnode(v);
    temp->add_leaves(leaflist);
    //current just replace 2
    if(!subroot->replace_child(2, temp)){
        return -1;
    }
    
    
    //now unmark the tree. recurse down the tree from the pertinent subroot until you hit an empty node or a leaf
    //subroot->unmark();
    return 0;
}


/*******************************************************************************
 * Function PQTree::mark(int value)
 * purpose: marks the pertinent subtree
 * returns the subroot of the pertinent subtree, otherwise NULL if an error occurs
 ********************************************************************************/
PQnode* PQTree::mark(int value){
    std::vector<Leaf*> fulls;
    
    //mark the full leaves based on the input value
    std::cout << "mark the full leaves\n";
    for(std::list<Leaf*>::iterator it=leaflist.begin(); it!=leaflist.end(); ++it){
        (*it)->set_mark(((*it)->get_value()==value)? full: empty);
        if((*it)->get_mark()==full){
            fulls.push_back((*it));
        }
    }
    
    //now find the parents of all the full leaves. add to the partials list but do not add duplicates
    std::cout << "create the list of partials\n";
    std::list<PQnode*> partials;
    for(size_t i=0; i<fulls.size(); ++i){
        PQnode *p = (PQnode*)fulls[i]->get_parent(); //this is the parent we want to add to the list of potential partials
        //add it into the partials list by inserting it at the correct position based on decreasing depth
        if(partials.empty()){
            partials.push_back(p);
        }else{
            std::cout << "list of partials\n";
            for(std::list<PQnode*>::iterator it=partials.begin(); it!=partials.end(); ++it){ //iterates through our list of partials
                (*it)->print();
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
    std::cout << "move up the tree\n";
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
    return NULL;
    
}

void PQTree::print_leaflist(){
    printf("printing the leaflist .........\n");
    for(std::list<Leaf*>::iterator it=leaflist.begin(); it!=leaflist.end(); ++it){
        if((*it)!=NULL){
            (*it)->print();
        }
    }
}

PQnode* PQTree::get_root(){ return root; }
std::list<Leaf*>* PQTree::get_leaflist(){ return &leaflist; }

void PQTree::print_expression(){
    root->print_expression();
    printf("\n");
}



//extend the functionality of the list class

int main(){
    
    std::cout << "Start of pq tree practice program!\n\n";
    int vec[] = {2, 3, 4};
    std::vector<int> v(vec, vec + sizeof(vec) / sizeof(int));
    PQTree tree(v); //defaults to a pnode
    v[0] = 5;
    //test the reduction
    tree.print_expression();
    tree.reduce_on(2, v);
    //tree.print();
    //tree.print_leaflist();
    tree.print_expression();
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
















