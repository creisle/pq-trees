
#ifndef __PQTree_H
#define __PQTree_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"

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
        PQnode *root; //pointer to the root of the PQ tree
        Node* build_from_expr(std::string const expr, size_t &i);
        PQnode* mark(std::vector<int> values);
        bool replace_full_with(Node *child);
        PQnode* reduce(std::vector<int> values);
        
    public:
        PQTree();
        PQTree(std::string const expression);
        PQTree(std::vector<int> v);
        virtual ~PQTree();
        
        //printing and testing functions
        void print(); //iterates recusively through the tree and prints out leaves and nodes
        void print_expression(bool mark = false); //prints an expression respresenting the tree structure and contents
        void print_leaflist(bool detail = false); //prints the contents of the leaflist
        
        //reduction opertaions
        bool reduce_and_replace(int value, std::vector<int> v); //planarity testing
        bool set_consecutive(std::vector<int> values); //for testing consectutive ones of a matrix
        
};

#endif
