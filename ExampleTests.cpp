

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include "PQTree.h"
#include <stdlib.h>
#include <sstream>

class ExampleTests : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ExampleTests );
    CPPUNIT_TEST( testPlanar );
    CPPUNIT_TEST( testConsectuive );
	CPPUNIT_TEST_SUITE_END();

	double m_value1;
	double m_value2;

public:

	void setUp()
	{
	  m_value1 = 2.0;
	  m_value2 = 3.0;
	}
    
    void tearDown()
    {
    }
    
    void testPlanar()
    {
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
        
        PQTree tree(adj[0]);
        
        for(size_t i=1; i<adj.size(); i++){
            int curr = (int)(i+1);
            std::vector<int> v = adj[i];
            if(!tree.reduce_and_replace(curr, v)){ fprintf(stderr, "error in building the and reducing the tree\n"); exit(1); }
        }
        std::string expected = "{ 0 }";
        std::string message = "expected { 0 } but found "+tree.print_expression()+"\n";
        CPPUNIT_ASSERT_MESSAGE( message, expected.compare(tree.print_expression())==0);
    }
    
    void testConsectuive() //purpose: tests a consectutive ones matrix example
    {
        bool pass = true;
        std::vector< std::vector<int> > mat = {
            {1, 2, 5}, //values that are one in our matrix
            {3, 4, 5},
            {1, 5},
            {3, 4},
            {2, 3}
        };
        PQTree tree("{1, 2, 3, 4, 5}");
        
        for(size_t i=0; i<mat.size(); ++i){
            if(!tree.set_consecutive(mat[i])){ pass = false; break;}
        }
        CPPUNIT_ASSERT_MESSAGE( "Consecutive ordering example failed\n ", pass);
    }
    
    void testLeafList()
    {
        
    }
};

int main( int argc, char **argv)
{
    
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( ExampleTests::suite() );
    runner.run();
    
    
    
    return 0;
}
