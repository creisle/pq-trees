/*
 * Testing framework
 * curently just write test's regularily.
 * learn how to use boost later this week
 *
 */

#include "test.h"


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
            
            std::vector< std::vector<int> > adj = {
                {2, 3, 4, 5, 6},
                {3, 6, 7},
                {4, 7},
                {5, 7},
                {7},
                {7}
            };
            
            printf("adding node %d\n", 1);
            PQTree tree(adj[0]);
            tree.print_expression();
            
            for(size_t i=1; i<adj.size(); i++){
                int curr = (int)(i+1);
                printf("adding node %d\n", curr);
                std::vector<int> v = adj[i];
                if(!tree.reduce_on(curr, v)){ return false; }
                tree.print_expression();
            }
            return true;
        }
        
        static bool test_consec_ones(){
            return false;
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
