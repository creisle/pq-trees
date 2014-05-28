#include <iostream>
#include <cstdlib>
#include <list>
#include <string>

using namespace std;

#define INIT_CHILDREN   3
#define FALSE -1;

enum nodetype {p , q, leaf};

//this will be the parent class for P-nodes, C-nodes and Q-nodes
//on the reduction step will ensure that we have a valid PQ tree
class Node{
    protected:
        Node* parent; //pointer to the parent node
        list<Node*> children; //empty list of children
        int value; //label for this node. generally only leaves will have labels
        nodetype type;
    
    public:
        
        Node(nodetype t, list<Node*> c, int v = 0, Node* par = NULL){ //default constructor
            parent = par;
            children = c;
            value = v;
            type = t;
            if(t==q&&c.size()<3){ //trying to initialize a q-node
                type = p;
            }
        }
        
        void print(){
            if(parent==NULL){
                cout << "parent: NULL\n";
            }else{
                cout << "parent: " << (*parent).get_type() << "\n";
            }
            
            cout << "type: " << type << "\n";
            list<Node*>::const_iterator it;
            for(it = children.begin(); it != children.end(); ++it){
                cout << "\nchildren";
                cout << (*it);
            }
        }
        
        nodetype get_type(){
            return type;
        }
        
        
};


class PQTree{
    
};

//extend the functionality of the list class

int main(){
    
    cout << "Start of pq tree practice program!\n";
    list<Node*> children;
    Node root(q, children); //try declaring a q node with less than 3 chidlren
    root.print();
    //attempt building a bunch of pnodes
    children.insert()
    
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
















