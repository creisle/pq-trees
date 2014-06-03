

#ifndef __PQNode_H
#define __PQNode_H

//dependencies
#include "Node.h"
#include "Leaf.h"

//forward declarations


class PQnode: public Node{
    private:
        std::list<Node*> children; //empty list of children
        
        //functions for tree reduction
        bool reduce_proot(); //for the subroot if it is a pnode
        bool reduce_qroot();
        bool reduce(bool direction); //for descedants of the pertinent subroot
        Node* group_children(std::list<Node*> group);
        
    public:
        //constructors and deconstructor
        PQnode();
        PQnode(std::vector<int> leaves, nodetype t = pnode);
        PQnode(Node *p, std::vector<int> leaves, nodetype t = pnode);
        virtual ~PQnode();
        
        //utility functions for the children
        void sort_children();
        bool replace_child(int value, Node::Node *tree);
        bool remove_child(int value);
        Node* pop_child();
        int count_children();
        
        
        //to do with the tree
        void add_leaves(std::list<Leaf*> &leaflist);
        int mark_node();
        void unmark();
        
        bool link_child(Node *child);
        void update_depth();
        
        
        //function for tree reduction
        bool reduce();
        
        //other
        void set_type(nodetype t);
        
        //mainly for testing
        void print_expression(bool m = false);
        void print();
        
};

#endif