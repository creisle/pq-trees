
#include "practice.h"

using namespace std; //never put this in the header file


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
    *(leaf_list_ptr) = NULL;
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

PQnode::PQnode(){
    type = pnode;
    parent = NULL;
    depth = 0;
}

PQnode::~PQnode(){
    for(size_t i = 0; i<children.size(); ++i){
        delete children[i];
    }
    children.clear();
}

/* every new node should start with a set of leaves.
 * should only link PQ nodes by replacing leaves in already existing nodes */
PQnode::PQnode(vector<int> leaves, nodetype t/* = pnode*/){
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
    for(size_t i=0; i<children.size(); i++){
        children[i]->print();
    }
    
}

void PQnode::add_leaves(vector<Leaf*> &leaflist){
    for(size_t i=0; i<children.size(); i++){
        if(children[i]->get_type()==leafnode){
            
            leaflist.push_back((Leaf*)children[i]);
            ((Leaf*)children[i])->set_leaf_list_ptr(&leaflist.back());
        }
    }
}

//check the children in order to mark the node
int PQnode::mark_node(){
    int fcount = 0;
    int pcount = 0;
    for(size_t i=0; i<children.size(); i++){
        switch(children[i]->get_mark()){
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

    
PQTree::PQTree(){
    root = NULL;
}

// non-default constructor will build a universal tree,
// i.e. p-node from a given set of input values
PQTree::PQTree(vector<int> set){
    root = new PQnode(set);
    root->add_leaves(leaflist);//add the new leaves to the leaflist...
    
}

//iterates recusively through the tree and prints out leaves and nodes
void PQTree::print(){
    if(root==NULL){
        cout << "Empty tree\n";
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
    vector<Leaf*> fulls;
    
    //mark the full leaves based on the input value
    cout << "mark the full leaves\n";
    for(size_t i=0; i<leaflist.size(); i++){
        leaflist[i]->set_mark((leaflist[i]->get_value()==value)? full: empty);
        if(leaflist[i]->get_mark()==full){
            fulls.push_back(leaflist[i]);
        }
    }
    
    //now find the parents of all the full leaves. add to the partials list but do not add duplicates
    cout << "create the list of partials\n";
    list<PQnode*> partials;
    for(size_t i=0; i<fulls.size(); ++i){
        PQnode *p = (PQnode*)fulls[i]->get_parent(); //this is the parent we want to add to the list of potential partials
        //add it into the partials list by inserting it at the correct position based on decreasing depth
        if(partials.empty()){
            partials.push_back(p);
        }else{
            cout << "list of partials\n";
            for(list<PQnode*>::iterator it=partials.begin(); it!=partials.end(); ++it){ //iterates through our list of partials
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
    cout << "move up the tree\n";
    while(partials.size()>1){
        //always deal with the front element first
        //mark the node
        PQnode *curr = partials.front();
        curr->mark_node();
        PQnode *p = (PQnode*)curr->get_parent(); //any parent in the tree will never be a leaf since they cannot have children. therefore this casting is safe
        partials.pop_front(); //remove the curr node and destroy the reference
        
        //add in the appropriate place. 
        for(list<PQnode*>::iterator it=partials.begin(); it!=partials.end(); ++it){ //iterates through our list of partials
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
    for(size_t i=0; i<leaflist.size(); i++){
        leaflist[i]->print();
    }
}


//extend the functionality of the list class

int main(){
    
    cout << "Start of pq tree practice program!\n\n";
    int vec[] = {2, 3, 4};
    vector<int> v(vec, vec + sizeof(vec) / sizeof(int));
    PQTree tree(v); //deafaults to a pnode
    //attempt building a bunch of pnodes
    tree.mark(2);
    tree.print();
    tree.mark(3);
    tree.print();
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
















