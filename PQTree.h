
#ifndef __PQTree_H
#define __PQTree_H

//dependencies
#include "PQnode.h"

//forward dependencies
class Node;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Class PQTree
 * purpose: implementation of booth and lueker's (1976) PQ tree
 * data structure as described by Sarah Young (1977)
 * specifically implemented for planarity testing. as such, some of the methods
 * will require some editing to use for consecutive ones
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class PQTree{
    private:
        std::list<Leaf*> leaflist; //list of the leaves for each given "value". first leaf in the list will
        PQnode *root; //pointer to the root of the PQ tree
    public:
        PQTree();
        PQTree(std::vector<int> set);
        virtual ~PQTree();
        //iterates recusively through the tree and prints out leaves and nodes
        void print();
        int reduce_on(int value, std::vector<int> v);
        //later make this method private so it can only be called from reduce
        PQnode* mark(int value);
        std::list<Leaf*>* get_leaflist();
        void print_leaflist();
        PQnode* get_root();
        void print_expression(bool mark = false);
        void clean_leaflist();
};

#endif