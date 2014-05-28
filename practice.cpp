#include <iostream>
#include <cstdlib>
#include <list>
#include <string>

using namespace std;

#define INIT_CHILDREN   3



//this will be the parent class for P-nodes, C-nodes and Q-nodes
class Node{
    
    Node* parent; //pointer to the parent node
    public:
        enum nodetype {p , q, leaf};
        Node(){ //default constructor
            parent = NULL;
        }
        virtual nodetype type() = 0; //is this the right way to declare an abstract function?
        
        virtual string print_details() = 0;
        
        string print(){
            string result = "";
            if(parent==NULL){
                result.append("parent: NULL\n");
            }else{
                result.append("parent: %d\n", (*parent).type());
            }
            result.append("type: %d\n", type());
            result.append(print_details());
            return result;
        }
        
        void link_child(Node *child){
            
        }
};

//this will be the class for P-nodes
class P: public Node{
    list<Node*> children; //empty list of children
    public:
        nodetype type(){
            return p;
        }
        
        string print_details(){
            list<Node*>::iterator it;
            string result = "children: ";
            for(it = children.begin(); it!= children.end(); ++it){
                result.append((**it).print());
            }
            result.append("\n");
            return result;
        }
        
        
};

//this will be the class for Q-nodes
class Q: public Node{
    list<Node*> children; //empty list of children
    public:
        nodetype type(){
            return q;
        }
        
        void print_subtree(){
            
        }
};

//this will be the class for leafs
class Leaf: public Node{
    int value;
    public:
        nodetype type(){
            return leaf;
        }
        Leaf(int v){
            value = v;
        }
        void set_value(int v){
            value = v;
        }
        int get_value(){
            return value;
        }
};

class PQTree{
    
};

int main(){
    cout << "Start of pq tree practice program!\n";
    
    //attempt building a bunch of pnodes
    P root; //deafault constuctor for the p node
    P a;
    root.link_child(a);
    cout << root.print();
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
















