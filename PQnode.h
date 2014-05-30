

#ifndef __PQNode_H
#define __PQNode_H

//dependencies
#include "Node.h"
#include "Leaf.h"

//forward declarations


class PQnode: public Node{
    std::list<Node*> children; //empty list of children
    public:
        PQnode();
        virtual ~PQnode();
        PQnode(std::vector<int> leaves, nodetype t = pnode);
        void print();
        void add_leaves(std::list<Leaf*> &leaflist);
        int mark_node();
        bool remove_child(int value);

};

#endif