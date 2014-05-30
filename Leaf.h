
#ifndef __LEAF_H
#define __LEAF_H

//dependencies
#include "Node.h"

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

#endif