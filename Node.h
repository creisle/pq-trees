
#ifndef __NODE_H
#define __NODE_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wc++98-compat"

//dependencies
#include <iostream>
#include <cstdlib>
#include <list>
#include <string>
#include <vector>

enum nodetype {pnode , qnode, leafnode};
enum marking {empty, partial, full};
enum print_option {option_none, option_marking, option_depth, option_src };
enum direction_type {left, right};

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
        virtual std::string print_expression(print_option m = option_none) = 0;
        virtual void update_depth() = 0;
        virtual bool less_than( Node& ) = 0;
        virtual void sort() = 0;
        virtual std::string convert_to_gml(int &) = 0;
        
        //getters
        Node* get_parent();
        int get_depth() const;
        nodetype get_type() const;
        marking get_mark() const;
        
        //setters
        virtual void unmark() = 0;
        void set_parent(Node*);
        
};

bool compare_marking(Node*, Node*);

#endif
