#include <iostream>
#include <cstdlib>
#include <list>
#include <string>
#include <vector>

using namespace std;

#define INIT_CHILDREN   3
#define FALSE -1;

enum nodetype {pnode , qnode, leafnode};


//this will be the parent class for P-nodes, C-nodes and Q-nodes
//on the reduction step will ensure that we have a valid PQ tree
class Node{
    Node* parent; //pointer to the parent node
    vector<Node> children; //empty list of children
    nodetype type; //type of node: q-node, p-node or leaf
    int value; //label for the node. generally only applicable to leaves
    int depth;
    
    
    public:
        
        Node(){ //default constructor
            parent = NULL;
            type = pnode;
            value = -1;
            depth = 0;
        }
        
        Node(nodetype t, vector<int> leaves){ //constructs a node with a given list of leaves
            parent = NULL;
            for(int i=0; i<leaves.size(); i++){
                Node temp(this, leaves[i]); // create the new leaf
                children.push_back(temp); // add the new leaf to the set of children
            }
            type = t;
            value = -1;
            if(t==qnode&&children.size()<3){ //trying to initialize a q-node
                type = pnode;
            }
            depth = 0;
        }
        
        //Note: although easier for testing b/c arrays can use explicit initialization. not always a safe function b/c uses sizeof
        Node(nodetype t, int leaves[], int num){ //constructs a node with a given list of leaves
            parent = NULL;
            for(int i=0; i<num; i++){
                Node temp(this, leaves[i]); // create the new leaf
                children.push_back(temp); // add the new leaf to the set of children
                
            }
            type = t;
            value = -1;
            if(t==qnode&&children.size()<3){ //trying to initialize a q-node
                type = pnode;
            }
            depth = 0;
        }
        
        /************************************************************************************************
         * Function: Node(Node* p, int v)
         * purpose: Construtor, constructs a leaf node.
         * input: pointer to the parent. leaf node cannot stand-alone, value. all leaves must be labelled
         ************************************************************************************************/
        Node(Node* p, int v){ 
            parent = p;
            type = leafnode;
            value = v;
            depth = p->depth + 1;
        }
        
        //this procedure is just for testing purposes
        void print(){
            printf("Node addr: %p\n", this);
            if(parent==NULL){
                printf("parent: NULL\n");
            }else{
                printf("parent: %p \n", parent);
            }
            
            printf("type: %d\nvalue: %d\ndepth: %d\nChildren: ", type, value, depth);
            if(children.size()==0){
                printf("none");
            }
            for(int i=0; i<children.size(); i++){
                printf("%d  ", (children[i]).value);
            }
            printf("\n");
        }
        
        //getter funtions
        nodetype get_type(){ return type; }
        Node* get_parent(){ return parent; }
        int get_value(){ return value; }
        vector<Node> get_children(){ return children; }
        
        
};

class PQnode: public Node{
    
};

class Leaf: public Node{
    
};



class PQTree{
    vector<Node*> leaves; //keeps track of all the leaves so we can do bottom-up search operations
    Node root; //constucts the root node using the default constructor
    public:
        PQTree(){ //default constructor
            
        }
        
        
        
        void update_depth(Node subroot){
            if(subroot.get_parent()==NULL){
                return;
            }
            for(int i=0; i<subroot.get_children().size(); i++){
                update_depth(subroot.get_children()[i]);
            }
        }
};

//extend the functionality of the list class

int main(){
    
    cout << "Start of pq tree practice program!\n";
    int v[] = {2, 3, 4};
    Node root(pnode, v, 3); //try declaring a q node with less than 3 chidlren
    root.print();
    //attempt building a bunch of pnodes
    
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
















