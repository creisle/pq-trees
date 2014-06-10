#PQ-Trees

**Note:** This software is distributed without any warranty or guarantee and is currently in development

Implementation of Booth and Leuker's (1) PQTree datastructure as described by Young (2). It should be noted that while the rules outlined for the PQTree trees were followed a large degree of creativity was used in implementing them

##What is a PQ Tree?

A PQ-Tree is a datastructure used in representing all permutations allowed on a given base set, U. The members of the base set are the leaves in the tree and the q-nodes and p-nodes are used in representing the different constraints we wish to apply. children of a p-node must be grouped under that node but their child elements may be permuted randomly with respect to one another. The q-node is more restrictive. Child elements of a q-node may only be reversed.

##How to use this implementation?

provided you are working on a unix or linux system, pull the files and use the makefile to compile

PQTrees can be contructed 3 ways:

    PQTree():           empty
    PQTree(string):     from an valid expression string
    PQTree(vector);     from an initial set of values to make a universal tree
    
PQTRee Expressions:

    pnode   - { }
    qnode   - [ ]
    leaf    - any integer

![Alt text](tree_example.jpg "PQTree example")

**Figure 1. Example PQTree. Can be represented by the expression: { 5 1 3 [ 3 {2 4 } [ { 4 5 } 6 2 ] ] }**
    
###PQTree class

####PQTree::PQTree()

    default constructor
    
####PQTree::PQTree(string const)

    non-default constructor. Initializes the PQTree from a valid PQTree expression

####PQTree::PQTree(vector\<int>)

    non-default constructor. Initializes the PQTree with a universal tree based on the 
    set of values in the input vector
####PQTree::print_expression(bool)

    prints out an expression corresponding to the current tree structure
####PQTree::reduce\_and_replace(int, vector\<int>)

    performs reductions on the tree based on the input vector. after reduction. replaces 
    the full leaves with the new universal tree that was built from the input vector
####PQTree::set\_consecutive(vector\<int>)

    Performs reductions on the tree based on the input vector. does not add or remove any 
    leaves from the tree
####PQTree::equivalent(PQTree&)

    Compares two trees to see if they are equivalent. Trees are equivalent if they impose 
    the same set of restaints.

###Examples
####Example: Testing Planarity of an st-numbered input graph from the adjacnecy list

    //adjacency matrix of an st-numbered input graph
    std::vector< std::vector<int> > adj =
    {
        {2, 3, 4, 5, 6},
        {3, 6, 7},
        {4, 7},
        {5, 7},
        {7},
        {7},
    };
        
    PQTree tree(adj[0]);
        
    for(size_t i=1; i<adj.size(); i++)
    {
        std::vector<int> v = adj[i];
        if(!tree.reduce_and_replace(curr, v))
        {
            fprintf(stderr, "error in building the and reducing the tree\n");
            return false;
        }
    }
    return true;

####Example: Testing Consecutive ones 

    std::vector< std::vector<int> > mat =
    {
        {1, 2, 5}, //values that are one in our matrix
        {3, 4, 5},
        {1, 5},
        {3, 4},
        {2, 3}
    };
    PQTree tree("{1, 2, 3, 4, 5}"); //create a tree with the base set
        
    for(size_t i=0; i<mat.size(); ++i) //apply the consecutive contraints in our matrix
    {
        if(!tree.set_consecutive(mat[i]))
        {
            return false;
        }
    }
    return true;

##References:

1. Booth, K.S., Lueker, G.S.: Testing for the consecutive ones property, interval graphs, and graph planarity using PQ-tree algorithms. J. Comput. Syst. Sci. 13, 335–379 (1976).
2. Young, S.M.: Implementation of PQ-tree algorithms, (1977).

