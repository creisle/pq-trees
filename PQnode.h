

#ifndef __PQNode_H
#define __PQNode_H

//dependencies
#include "Node.h"
#include "Leaf.h"

//forward declarations


class PQnode: public Node{
    std::list<Node*> children; //empty list of children
    public:
        //constructors and deconstructor
        PQnode();
        PQnode(std::vector<int> leaves, nodetype t = pnode);
        PQnode(Node *p, std::vector<int> leaves, nodetype t = pnode);
        virtual ~PQnode();
        
        //testing for reducible template functions
        bool q_reducible();
        bool p_reducible();
        bool subroot_p_reducible();
        bool subroot_q_reducible();
        
        //utility functions
        void sort_children();
        void print();
        void add_leaves(std::list<Leaf*> &leaflist);
        int mark_node();
        bool remove_child(int value);
        void unmark();
        bool replace_child(int value, Node::Node *tree);
        void print_expression();
};

#endif