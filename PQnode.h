

#ifndef __PQNode_H
#define __PQNode_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++11-extensions"

//dependencies
#include "Node.h"
#include "Leaf.h"

//forward declarations

/**** enums from Node class
enum print_option {option_none, option_marking, option_depth, option_src };
enum nodetype {pnode , qnode, leafnode};
enum marking {empty, partial, full};
*/


class PQnode: public Node
{
    private:
        std::list<Node*> children; //empty list of children
        
        //functions for tree reduction
        bool reduce_proot(); //for the subroot if it is a pnode
        bool reduce_qroot();
        bool qreduce(direction_type);
        bool preduce(direction_type);
        bool reduce(direction_type); //for descedants of the pertinent subroot
        Node* group_children(std::list<Node*> &);
        size_t grab_marks(std::list<Node*>::iterator &, marking, std::list<Node*>&);
        bool promote_partial_children(std::list<Node*>::iterator&, direction_type);
        void pop_children(std::list<Node*> &kids);
        
        //funcitons for marking
        size_t skip_marks(std::list<Node*>::iterator &, marking);
        
        //other
        bool less_than(Node&);
        
    public:
        //constructors and deconstructor
        PQnode();
        PQnode(std::vector<int>, std::list<Leaf*>&, nodetype t = pnode); //default src = -1
        PQnode(std::vector<int>, std::list<Leaf*>&, int src, nodetype t = pnode); //sepcific source
        
        virtual ~PQnode();
        
        //utility functions for the children
        void sort_children();
        Node* pop_child();
        size_t count_children();
        
        
        //to do with the tree
        bool mark();
        void unmark();
        std::list<int> condense_and_replace(Node*);
        bool link_child(Node*, direction_type dir = right); //DANGEROUS!
        void update_depth();
        
        
        //function for tree reduction
        bool reduce();
        
        //other
        void set_type(nodetype t);
        void sort();
        
        //mainly for testing
        std::string print_expression(print_option m = option_none);
        void print();
};

namespace custom
{
    void print(std::list<int>);
    void print(std::vector<int>);
}

#endif
