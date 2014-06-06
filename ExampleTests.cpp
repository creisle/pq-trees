

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

class PQTreeTests : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PQTreeTests );
    CPPUNIT_TEST( testPlanar );
    CPPUNIT_TEST( testConsectuive );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
	}
    
    void tearDown()
    {
    }
    
    void testPlanar()
    {
        //adjacency matrix of an st-numbered input graph
        std::vector< std::vector<int> > adj =
        {
            {2, 3, 4, 5, 6},
            {3, 6, 7},
            {4, 7},
            {5, 7},
            {7},
            {7},
            {0} // 0 is not actually a node. but just so we can check the final reduction
        };
        
        PQTree tree(adj[0]);
        
        for(size_t i=1; i<adj.size(); i++)
        {
            int curr = (int)(i+1);
            std::vector<int> v = adj[i];
            if(!tree.reduce_and_replace(curr, v))
            {
                fprintf(stderr, "error in building the and reducing the tree\n");
                exit(1);
            }
        }
        std::string expected = "{ 0 }";
        std::string message = "expected { 0 } but found "+tree.print_expression()+"\n";
        CPPUNIT_ASSERT_MESSAGE( message, expected.compare(tree.print_expression())==0);
    }
    
    void testConsectuive() //purpose: tests a consectutive ones matrix example
    {
        bool pass = true;
        std::vector< std::vector<int> > mat =
        {
            {1, 2, 5}, //values that are one in our matrix
            {3, 4, 5},
            {1, 5},
            {3, 4},
            {2, 3}
        };
        PQTree tree("{1, 2, 3, 4, 5}");
        
        for(size_t i=0; i<mat.size(); ++i)
        {
            if(!tree.set_consecutive(mat[i]))
            {
                pass = false; break;
            }
        }
        CPPUNIT_ASSERT_MESSAGE( "Consecutive ordering example failed\n ", pass);
    }
};

class LeafTests : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LeafTests );
    CPPUNIT_TEST( testLeafList );
    CPPUNIT_TEST( test_marking );
    CPPUNIT_TEST( test_update_depth );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
	}
    
    void tearDown()
    {
    }
    
    void testLeafList() //tests adding leaves to ensure they add to the list you pass in correctly
    {
        std::list<Leaf*> lfs;
        for(int i=0; i<5; i++){
            new Leaf(i, lfs);
        }
        int count = lfs.size();
        CPPUNIT_ASSERT_EQUAL(count, 5);
        lfs.clear();
    }
    
    void test_marking() //tests the marking functionality of the leaf class
    {
        std::list<Leaf*> lfs;
        Leaf *lf = new Leaf(1, lfs);
        lf->mark();
        CPPUNIT_ASSERT(lf->get_mark()==full);
        lf->unmark();
        CPPUNIT_ASSERT(lf->get_mark()==empty);
        delete lf;
    }
    
    void test_update_depth()
    {
        std::list<Leaf*> lfs;
        PQnode *parent = new PQnode(); //depth will default to zero
        Leaf *lf = new Leaf(1, lfs); //depth defaults to zero
        CPPUNIT_ASSERT_EQUAL(lf->get_depth(), 0);
        parent->link_child(lf); //this will call update depth on the leaf node
        CPPUNIT_ASSERT_EQUAL(lf->get_depth(), 1);
        delete parent;
    }
};

class PQnodeTests : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PQnodeTests );
    CPPUNIT_TEST( testLeafList );
    CPPUNIT_TEST( test_marking );
    CPPUNIT_TEST( test_update_depth );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
	}
    
    void tearDown()
    {
    }
    
    void testLeafList() //tests adding leaves to ensure they add to the list you pass in correctly
    {
        
    }
    
    void test_marking() //tests the marking functionality of the leaf class
    {
        
    }
    
    void test_update_depth()
    {
        
    }
};

int main( int argc, char **argv)
{
    
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( PQTreeTests::suite() );
    runner.addTest( LeafTests::suite() );
    runner.run();
    
    
    
    return 0;
}
