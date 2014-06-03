
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
        Node* build_from_expr(std::string const expr, int &i);
        PQnode* mark(int value);
        void clean_leaflist();
        bool replace_full_with(std::string const tree_in);
        
    public:
        PQTree();
        PQTree(std::string const expression);
        virtual ~PQTree();
        
        //printing and testing functions
        void print(); //iterates recusively through the tree and prints out leaves and nodes
        void print_expression(bool mark = false); //prints an expression respresenting the tree structure and contents
        void print_leaflist(bool detail = false); //prints the contents of the leaflist
        
        //reducion opertaions
        bool reduce_on(int value, std::string const tree_in);
};

#endif