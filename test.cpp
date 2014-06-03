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
        
        static void test_leaflist(){
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
    Tests::test_build_from_expr();
    
    
    return EXIT_SUCCESS; //indicates the the program ran successfully
}