

#ifndef __PQNode_H
#define __PQNode_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++11-extensions"

//dependencies
#include "Node.h"
#include "Leaf.h"

//forward declarations


class PQnode: public Node
{
    private:
        std::list<Node*> children; //empty list of children
        
        //functions for tree reduction
        bool reduce_proot(); //for the subroot if it is a pnode
        bool reduce_qroot();
        bool qreduce(bool direction);
        bool preduce(bool direction);
        bool reduce(bool direction); //for descedants of the pertinent subroot
        Node* group_children(std::list<Node*> &group);
        size_t grab_marks(std::list<Node*>::iterator &itr, marking m, std::list<Node*> &tmp);
        bool promote_partial_children(std::list<Node*>::iterator &it, bool direction);
        void pop_children(std::list<Node*> &kids);
        
        //funcitons for marking
        size_t skip_marks(std::list<Node*>::iterator &itr, marking mark);
        
    public:
        //constructors and deconstructor
        PQnode();
        PQnode(std::vector<int> leaves, std::list<Leaf*> &leaflist, nodetype t = pnode);
        virtual ~PQnode();
        
        //utility functions for the children
        void sort_children();
        Node* pop_child();
        size_t count_children();
        
        
        //to do with the tree
        bool mark();
        void unmark();
        bool condense_and_replace(Node *child);
        bool link_child(Node *child); //DANGEROUS!
        void update_depth();
        
        
        //function for tree reduction
        bool reduce();
        
        //other
        void set_type(nodetype t);
        bool is_equivalent(Node *other);
        //bool equivalent(PQnode &curr, std::string::iterator &it);
        
        //mainly for testing
        std::string print_expression(print_option m = none);
        void print();
        
};

#endif
