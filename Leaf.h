
#ifndef __LEAF_H
#define __LEAF_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wexit-time-destructors"
#pragma GCC diagnostic ignored "-Wglobal-constructors"


//dependencies
#include "Node.h"
#include <string>

class Leaf: public Node
{
    private:
        int value;
        Leaf **leaf_list_ptr;
    public:
        //constructors and destructors
        Leaf(Node *p, int v, std::list<Leaf*> &leaflist);
        Leaf(int v, std::list<Leaf*> &leaflist);
        virtual ~Leaf();
        
        //testing functions
        void print();
        std::string print_expression(bool m = false);
        
        //update and setters
        void mark();
        void update_depth();
        void unmark();
        
        //getters
        int get_value();
};

#endif
