#include <iostream>
#include <cstdlib>
#include <list>
#include <string>
#include <vector>

using namespace std;

enum nodetype {pnode , qnode};


//this will be the parent class for P-nodes, C-nodes and Q-nodes
//on the reduction step will ensure that we have a valid PQ tree
class Node{
    protected:
        Node* parent; //pointer to the parent node
        int depth;
    public:
        
        Node(){ //default constructor
            parent = NULL;
            depth = 0;
        }
        
        //this procedure is just for testing purposes
        virtual void print(){
            printf("Node addr: %p\n", this);
            if(parent==NULL){
                printf("parent: NULL\n");
            }else{
                printf("parent: %p \n", parent);
            }
        }
        
        //getter funtions
        Node* get_parent(){ return parent; }
        int get_depth(){ return depth; }
        
};

class Leaf: public Node{
    int value;
    Node *nextleaf;
    public:
        Leaf(Node *p, int v){
            parent = p;
            value = v;
            depth = p->get_depth() +1;
        }
        Node* get_next(){ return nextleaf; }
        void set_next(Node *next){ nextleaf = next; }
        void print(){
            printf("node-type: LEAF NODE\n");
            Node::print();
            printf("value: %d\n", value);
        }
};

class PQnode: public Node{
    vector<Node*> children; //empty list of children
    nodetype type; //specifies whether this is a p or q node
    public:
        PQnode(){
            type = pnode;
            parent = NULL;
        }
        /* every new node should start with a set of leaves.
         * should only link PQ nodes by replacing leaves in already existing nodes */
        PQnode(vector<int> leaves, nodetype t){
            for(int i=0; i<leaves.size(); i++){
                Leaf *temp = new Leaf(this, leaves[i]); // create the new leaf
                children.push_back(temp); // add the new leaf to the set of children
            }
        }
        
        PQnode(int leaves[], int num, nodetype t){
            for(int i=0; i<num; i++){
                Leaf *temp = new Leaf(this, leaves[i]); // create the new leaf
                children.push_back(temp); // add the new leaf to the set of children
            }
        }
        
        void print(){
            printf("node-type: %s\n", (type==pnode)? "P": "Q");
            Node::print();
            printf("num children: %lu\n", children.size());
            for(int i=0; i<children.size(); i++){
                children[i]->print();
            }
        }

};

class PQTree{
    
};


//extend the functionality of the list class

int main(){
    
    cout << "Start of pq tree practice program!\n";
    int v[] = {2, 3, 4};
    PQnode root(v, 3, pnode); //try declaring a q node with less than 3 chidlren
    root.print();
    //attempt building a bunch of pnodes
    
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
















