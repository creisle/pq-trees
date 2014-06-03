
#ifndef __LEAF_H
#define __LEAF_H

//dependencies
#include "Node.h"

class Leaf: public Node{
    int value;
    Leaf **leaf_list_ptr;
    public:
        //constructors and destructors
        //Leaf(Node *p, int v);
        Leaf(Node *p, int v, std::list<Leaf*> &lflist);
        virtual ~Leaf();
        
        //testing functions
        void print();
        void print_expression(bool m = false);
        
        //update and setters
        void mark();
        void update_depth();
        //void set_leaf_list_ptr(Leaf** ptr);
        void unmark();
        
        //getters
        int get_value();
        //Leaf** get_leaf_list_ptr();
};

#endif