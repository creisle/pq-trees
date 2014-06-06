
#ifndef __NODE_H
#define __NODE_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"

//dependencies
#include <iostream>
#include <cstdlib>
#include <list>
#include <string>
#include <vector>

enum nodetype {pnode , qnode, leafnode};
enum marking {empty, partial, full};

class Node
{
    protected:
        Node* parent; //pointer to the parent node
        int depth;
        nodetype type;
        marking node_mark;
        
    public:
        Node();
        virtual ~Node(){} //destructor. use this to get rid of crap "new"'d inside the constructor
        virtual void print() = 0; //now this is pure virtual
        virtual std::string print_expression(bool m = false) = 0;
        virtual void update_depth() = 0;
        
        //getters
        Node* get_parent();
        int get_depth() const;
        nodetype get_type() const;
        marking get_mark() const;
        
        //setters
        virtual void unmark();
        void set_parent(Node *p);
        
};

bool compare_marking(Node *first, Node *second);

#endif
