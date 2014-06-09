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
    
##Ways to use the trees?

1. finding interval graphs using the set_consecutive(vector) function
2. testing planarity of st-numbered graphs using the reduce_and_replace(int, vector)

##References:

1. Booth, K.S., Lueker, G.S.: Testing for the consecutive ones property, interval graphs, and graph planarity using PQ-tree algorithms. J. Comput. Syst. Sci. 13, 335–379 (1976).
2. Young, S.M.: Implementation of PQ-tree algorithms, (1977).

