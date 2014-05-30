
#include <iostream>
#include <cstdlib>
#include <list>
#include <string>
#include <vector>

enum nodetype {pnode , qnode, leafnode};
enum marking {empty, partial, full};

class Node{
    protected:
        Node* parent; //pointer to the parent node
        int depth;
        nodetype type;
        marking mark;
    public:
        Node();
        virtual ~Node(){} //destructor. use this to get rid of crap "new"'d inside the constructor
        virtual void print() = 0; //now this is pure virtual
        Node* get_parent();
        int get_depth();
        nodetype get_type();
        marking get_mark();
};

class Leaf: public Node{
    int value;
    Leaf **leaf_list_ptr;
    public:
        Leaf(int v);
        Leaf(Node *p, int v);
        virtual ~Leaf();
        void print();
        void set_mark(marking m);
        int get_value();
        void set_leaf_list_ptr(Leaf** ptr);
        Leaf** get_leaf_list_ptr();
};


class PQnode: public Node{
    std::list<Node*> children; //empty list of children
    public:
        PQnode();
        virtual ~PQnode();
        PQnode(std::vector<int> leaves, nodetype t = pnode);
        void print();
        void add_leaves(std::list<Leaf*> &leaflist);
        int mark_node();
        bool remove_child(int value);

};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Class PQTree
 * purpose: implementation of booth and lueker's (1976) PQ tree
 * data structure as described by Sarah Young (1977)
 * specifically implemented for planarity testing. as such, some of the methods
 * will require some editing to use for consecutive ones
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class PQTree{
    std::list<Leaf*> leaflist; //list of the leaves for each given "value". first leaf in the list will
    PQnode *root; //pointer to the root of the PQ tree
    
    public:
        PQTree();
        PQTree(std::vector<int> set);
        virtual ~PQTree();
        //iterates recusively through the tree and prints out leaves and nodes
        void print();
        void reduce_on(int value);
        //later make this method private so it can only be called from reduce
        void mark(int value);
        
        void print_leaflist();
        
        PQnode* get_root();
        
        
};
