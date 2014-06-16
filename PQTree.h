
#ifndef __PQTree_H
#define __PQTree_H

//warning to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++11-extensions"

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
class PQTree
{
    private:
        PQnode *root; //pointer to the root of the PQ tree
        std::list<Leaf*> leaflist; //don't need to add to the destructor since this is never "newed"
        
        //testing
        Node* build_from_expr(std::string const expr, size_t &i);
        
        //marking
        PQnode* mark(std::vector<int> values);
        void add_unique_by_depth(PQnode *p, std::list<PQnode*> &parents);
        std::list<Leaf*> mark_pertinent(std::vector<int> values);
        
        //reduction
        std::list<int> replace_full_with(Node*);
        PQnode* reduce(std::vector<int>);
        std::list<Leaf*> get_pertinent();
        
    public:
        
        //constructors and destructor
        PQTree();
        PQTree(std::string const);
        PQTree(std::vector<int>, int src=-1);
        virtual ~PQTree();
        
        //printing and testing functions
        void print(); //iterates recusively through the tree and prints out leaves and nodes
        std::string print_expression(print_option p = option_none); //prints an expression respresenting the tree structure and contents
        void print_leaflist(bool detail = false); //prints the contents of the leaflist
        
        //reduction opertaions
        std::list<int> reduce_and_replace(int, std::vector<int>); //planarity testing
        bool set_consecutive(std::vector<int>); //for testing consectutive ones of a matrix
        
        //leaflist functions
        size_t get_leaflist_size();
        
        //comparing
        bool equivalent(PQTree&);
};

namespace custom
{
    bool compare(std::string s1, std::string s2);
    bool contains(std::vector<int> vec, int v);
};

#endif
