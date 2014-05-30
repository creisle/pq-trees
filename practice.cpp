/*
 * Note: the leaflist list is referenced by other variables in the program. list nodes should
 * have the same ref throughout the program until they are destroyed
 * however if for some reason thier addr changes this would be a problem
 * but i really don't think this would happen
 */

#include "practice.h"

//using namespace std; //never put this in the header file


//this will be the parent class for P-nodes, C-nodes and Q-nodes
//on the reduction step will ensure that we have a valid PQ tree
Node::Node(){ //default constructor
    parent = NULL;
    depth = 0;
    mark = empty;
}

//this procedure is just for testing purposes
// paritally abstract?
void Node::print(){
    printf("Node addr: %p\n", this);
    if(parent==NULL){
        printf("parent addr: NULL\n");
    }else{
        printf("parent addr: %p \n", parent);
    }
    switch(mark){
        case full:
            printf("mark: full\n");
            break;
        case empty:
            printf("mark: empty\n");
            break;
        case partial:
            printf("mark: partial\n");
            break;
    }
    
}
        
//getter funtions
Node* Node::get_parent(){ return parent; }
int Node::get_depth(){ return depth; }
nodetype Node::get_type(){ return type; }
marking Node::get_mark(){ return mark; }


Leaf::Leaf(int v): Node(), leaf_list_ptr(NULL){
    value = v;
    type = leafnode;
}
Leaf::Leaf(Node *p, int v): leaf_list_ptr(NULL){
    parent = p;
    value = v;
    depth = p->get_depth() +1;
    type = leafnode;
}

Leaf::~Leaf(){
    //follow pointer to the leaflist entry to null it's pointer
    *leaf_list_ptr = NULL;
    leaf_list_ptr = NULL;
}

void Leaf::print(){
    printf("========= node-type: LEAF NODE ======== \n");
    Node::print();
    printf("value: %d\n\n", value);
}
void Leaf::set_mark(marking m){ mark = m;}
int Leaf::get_value(){ return value; }
void Leaf::set_leaf_list_ptr(Leaf** ptr){ leaf_list_ptr = ptr; }
Leaf** Leaf::get_leaf_list_ptr(){ return leaf_list_ptr; }

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

void PQnode::add_leaves(std::list<Leaf*> &leaflist){
    for(std::list<Node*>::iterator it=children.begin(); it!=children.end(); ++it){
        if(Leaf *lf = dynamic_cast<Leaf*>((*it))){ //if the child is a leaf
            leaflist.push_back(lf);
            lf->set_leaf_list_ptr(&leaflist.back());
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

/* procedure: reduce_on(int value)
 * purpose: marks the nodes as full or empty based on the given value
 * then applies the templates to enforce consecutive ordering of the nodes with this value
 * returns -1 if the reduciton step fails
 * returns 0 if the reduction step is successfull.
 */
void PQTree::reduce_on(int value){
    
}

//later make this method private so it can only be called from reduce
void PQTree::mark(int value){
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
    }
    
}

void PQTree::print_leaflist(){
    for(std::list<Leaf*>::iterator it=leaflist.begin(); it!=leaflist.end(); ++it){
        if((*it)!=NULL){
            (*it)->print();
        }
    }
}

PQnode* PQTree::get_root(){ return root; }

//extend the functionality of the list class

int main(){
    
    std::cout << "Start of pq tree practice program!\n\n";
    int vec[] = {2, 3, 4};
    std::vector<int> v(vec, vec + sizeof(vec) / sizeof(int));
    PQTree tree(v); //deafaults to a pnode
    std::cout << "testing the remove method\n";
    PQnode *temp = tree.get_root();
    if(temp)
        std::cout << "got the root\n";
    temp->print();
    temp->remove_child(3);
    std::cout << "\n after removing the child the tree looks like ..... \n" << std::endl;
    tree.print();
    std::cout << "now printing the leaflist\n" << std::endl;
    tree.print_leaflist();
    
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
















