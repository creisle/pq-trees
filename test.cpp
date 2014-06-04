/*
 * Testing framework
 * curently just write test's regularily.
 * learn how to use boost later this week
 *
 */

#include "test.h"

std::string print_vec(std::vector<int> v);

std::string print_vec(std::vector<int> v){
    std::ostringstream temp;
    temp << "{ ";
    for(size_t i=0; i<v.size(); ++i){
        temp << v[i] << " ";
    }
    temp << "}";
    return temp.str();
}

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
            
            //adjacency matrix of an st-numbered input graph
            std::vector< std::vector<int> > adj = {
                {2, 3, 4, 5, 6},
                {3, 6, 7},
                {4, 7},
                {5, 7},
                {7},
                {7},
                {0} // 0 is not actually a node. but just so we can check the final reduction
            };
            
            printf("adding node %d\n", 1);
            PQTree tree(adj[0]);
            tree.print_expression();
            
            for(size_t i=1; i<adj.size(); i++){
                int curr = (int)(i+1);
                printf("adding node %d\n", curr);
                std::vector<int> v = adj[i];
                if(!tree.reduce_and_replace(curr, v)){ return false; }
                tree.print_expression();
            }
            return true;
        }
        
        static bool test_consec_ones(){
            printf("\nTESTING: \n");
            printf("purpose: tests a consectutive ones matrix example\n");
            std::vector< std::vector<int> > mat = {
                {1, 2, 5}, //values that are one in our matrix
                {3, 4, 5},
                {1, 5},
                {3, 4},
                {2, 3}
            };
            PQTree tree("{1, 2, 3, 4, 5}");
            tree.print_expression(true);
            
            for(size_t i=0; i<mat.size(); ++i){
                int curr = (int)(i+1);
                printf("adding the contraints from row %d: %s\n", curr, print_vec(mat[i]).c_str());
                if(!tree.set_consecutive(mat[i])){ return false; }
                tree.print_expression(true);
            }
            
            return false;
        }
        
    
};


int main(){
    
    std::cout << "Start of pq tree practice program!\n\n";
    //Tests::test_planar();
    Tests::test_consec_ones();
    
    
    return EXIT_SUCCESS; //indicates the the program ran successfully
}
