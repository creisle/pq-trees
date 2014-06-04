/*
 * Testing framework
 * curently just write test's regularily.
 * learn how to use boost later this week
 *
 */

#include "PQTree.h"
#include <stdlib.h>


class Tests{
    public:
        static void test_build_from_expr(){
            printf("TESTING: PQTree(std::string const expression)\n");
            printf("purpose: tests building the tree using expressions\n");
            std::string exp = "{ 2 3 [ 6 7 8] 4 5 {6 8}}";
            printf("input: %s\n", exp.c_str());
            PQTree tree(exp);
            printf("output: ");
            tree.print_expression();
            printf("leaflist: ");
            tree.print_leaflist();
        }
        
        static bool test_planar(){
            printf("TESTING: \n");
            printf("purpose: tests a planar graph example\n");
            
            printf("adding node 1\n");
            PQTree tree("{2 3 4 5 6}");
            printf("EXPECT\t{ 2  3  4  5  6 }\nFOUND:\t");
            tree.print_expression();
            
            printf("adding node 2\n");
            if(!tree.reduce_on(2, "{3 6 7}")){ return false; }
            printf("EXPECT:\t{ 3  4  5  6  { 3  6  7 } }\nFOUND:\t");
            tree.print_expression();
            
            printf("adding node 3\n");
            if(!tree.reduce_on(3, "{4 7}")){ return false; }
            printf("EXPECT: { { { 6 7 } { 4 7 } } 4 5 6 }\t\nFOUND:\t");
            tree.print_expression();
            
            printf("adding node 4\n");
            if(!tree.reduce_on(4, "{5 7}")){ return false; }
            printf("EXPECT:\t{ [ { 6 7 } 7 { 5 7 } ] 5 6 }\nFOUND:\t");
            tree.print_expression();
            
            printf("adding node 5\n");
            if(!tree.reduce_on(5, "{7}")){ return false; }
            printf("EXPECT:\t{ }\nFOUND:\t");
            tree.print_expression();
            
            printf("adding node 6\n");
            if(!tree.reduce_on(6, "{7}")){ return false; }
            printf("EXPECT:\t{ }\nFOUND:\t");
            tree.print_expression();
            
            return true;
        }
    
};


int main(){
    
    std::cout << "Start of pq tree practice program!\n\n";
    /*int vec[] = {2, 3, 4};
    std::vector<int> v(vec, vec + sizeof(vec) / sizeof(int));
    PQTree tree(v); //defaults to a pnode
    v[0] = 5;
    //test the reduction
    tree.reduce_on(2, v);
    */
    //Tests::test_build_from_expr();
    Tests::test_planar();
    
    
    return EXIT_SUCCESS; //indicates the the program ran successfully
}