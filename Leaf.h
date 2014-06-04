
#ifndef __LEAF_H
#define __LEAF_H

//dependencies
#include "Node.h"

class Leaf: public Node{
    
    private:
        int value;
        Leaf **leaf_list_ptr;
    public:
        //constructors and destructors
        Leaf(Node *p, int v);
        virtual ~Leaf();
        
        //testing functions
        void print();
        void print_expression(bool m = false);
        
        //update and setters
        void mark();
        void update_depth();
        void unmark();
        
        //getters
        int get_value();
        
        static std::list<Leaf*> get_pertinent();
        static std::list<Leaf*> mark_pertinent(int value);
};

#endif