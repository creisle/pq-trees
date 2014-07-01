
#ifndef __LEAF_H
#define __LEAF_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wc++98-compat"


//dependencies
#include "Node.h"
#include <string>

/**** enums from Node class
enum print_option {option_none, option_marking, option_depth, option_src };
enum nodetype {pnode , qnode, leafnode};
enum marking {empty, partial, full};
*/

class Leaf: public Node
{
    private:
        int value;
        int source; //value used to keep track for when we produce the embedding
        Leaf **leaf_list_ptr;
        bool less_than(Node&);
        void sort(){}
        
    public:
        //constructors and destructors
        Leaf(Node*, int, std::list<Leaf*>&, int src = -1);
        Leaf(int, std::list<Leaf*>&, int src = -1);
        virtual ~Leaf();
        
        //testing functions
        void print();
        std::string print_expression(print_option m = option_none);
        std::string convert_to_gml(int&);
        
        //update and setters
        void mark();
        void update_depth();
        void unmark();
        
        //getters
        int get_value();
        int get_source();
};

#endif
