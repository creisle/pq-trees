

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

#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wpadded"



class PQTreeTests : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PQTreeTests );
    CPPUNIT_TEST( testPlanar );
    CPPUNIT_TEST( testConsectuive );
    CPPUNIT_TEST( testConstructExpression );
    CPPUNIT_TEST( testEquivalent );
	CPPUNIT_TEST_SUITE_END();

public:
    
    //assumes the input adjacency matrix starts numbering vertices at 1
    bool testPlanar(std::vector< std::vector<int> > adj, PQTree **tp)
    {
        PQTree *tree = new PQTree(adj[0], 1);
        
        for(size_t i=1; i<adj.size(); i++)
        {
            int curr = (int)(i+1);
            std::vector<int> v = adj[i];
            bool result = tree->reduce_and_replace(curr, v); 
            if(!result)
            {
                fprintf(stderr, "error in building the and reducing the tree\n");
                return false;
            }
        }
        *tp = tree;
        return true;
    }
    
    void testPlanar()
    {
        //adjacency matrix of an st-numbered input graph
        PQTree *tree = NULL;
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
        
        
        bool result = testPlanar(adj, &tree);
        
        std::string exp = "{ 0 }";
        
        CPPUNIT_ASSERT_MESSAGE("Expected: "+exp+"\nFound: "+tree->print_expression()+"\n", custom::compare(exp, tree->print_expression()));
        CPPUNIT_ASSERT(result);
        
        delete tree;
        tree = NULL;
        
        std::vector< std::vector<int> > adj2 =
        {
            {2, 3, 10}, //1
            {3, 6, 8},  //2
            {4, 10},    //3
            {5, 6, 10}, //4
            {6, 9},     //5
            {7, 9},     //6
            {8, 9},     //7
            {9},        //8
            {10},       //9
        };
        result = testPlanar(adj2, &tree);
        
        exp = "{ 10  [ 10  10  10 ] }";
        CPPUNIT_ASSERT(result);
        CPPUNIT_ASSERT_MESSAGE("Expected: "+exp+"\nFound: "+tree->print_expression()+"\n", custom::compare(exp, tree->print_expression()));
        delete tree;
        tree = NULL;
    }
    
    void testConsectuive() //purpose: tests a consectutive ones matrix example
    {
        bool pass = true;
        std::vector< std::vector<int> > mat =
        {
            {2, 3, 4}, //values that are one in our matrix
            {1, 2, 3},
            {4, 5},
            {2, 3},
            {3, 4},
            {1},
            {5}
        };
        PQTree *tree = new PQTree("{1, 2, 3, 4, 5}");
        
        for(size_t i=0; i<mat.size(); ++i)
        {
            if(!tree->set_consecutive(mat[i]))
            {
                pass = false; break;
            }
        }
        delete tree;
        CPPUNIT_ASSERT_MESSAGE( "Consecutive ordering example failed\n ", pass);
    }
    
    void testConstructExpression() //test PQnode(std::vector<int> leaves, std::list<Leaf*> &leaflist, nodetype t = pnode); adds to the leaflist correctly
    {
        //mock a tre from an expression and see that the output matches
        std::string expr = "[ 5  [ 2  3  { 3 4 } ] 1  { 2  3  { 4 5 } } ]";
        size_t expr_count = 10;
        PQTree *tree = new PQTree(expr);
        
        std::string message = "expected "+expr+" but found "+tree->print_expression()+"\n";
        CPPUNIT_ASSERT_MESSAGE( message, custom::compare(expr, tree->print_expression()));
        
        size_t count = tree->get_leaflist_size();
        delete tree;
        
        CPPUNIT_ASSERT_EQUAL(expr_count, count);
        
    }
    
    void testEquivalent()
    {
        PQTree a("{ 3 4 5 { 6 7 8 } 9 }");
        PQTree b("{ 3 5 4 { 6 7 8 } 9 }");
        PQTree c("[ 3 5 4 { 6 7 8 } 9 ]");
        PQTree d("{ 3 5 4 [ 6 7 8 ] 9 }");
        
        PQTree e("{ 3 5 { 7 8 9 } { 6 7 8 } 9 }");
        PQTree f("{ 3 5 { 6 7 8 } { 8 7 9 } 9 }");
        PQTree g("{ 3 5 [ 7 8 9 ] { 6 7 8 } 9 }");
        PQTree h("{ 3 5 { 6 7 8 } [ 8 7 9 ] 9 }");
        PQTree i("{ 3 5 { 6 7 8 } [ 9 7 8 ] 9 }");
        
        PQTree j("[ 9 { 6 7 8 } 4 5 3]");
        
        CPPUNIT_ASSERT_MESSAGE( "pnode reordering, should be equivalent\n", a.equivalent(b));
        CPPUNIT_ASSERT_MESSAGE( "root qnode should NOT be equivalent\n", !b.equivalent(c));
        CPPUNIT_ASSERT_MESSAGE( "descendant qnode should NOT be equivalent\n", !b.equivalent(d));
        CPPUNIT_ASSERT_MESSAGE( "pnode reordering, should be equivalent. found e: "+e.print_expression()+" and f: "+f.print_expression()+"\n", e.equivalent(f));
        CPPUNIT_ASSERT_MESSAGE( "qnode reordering, should NOT be equivalent. found g: "+g.print_expression()+" and h: "+h.print_expression()+"\n", !g.equivalent(h));
        CPPUNIT_ASSERT_MESSAGE( "qnode flip, should be equivalent. found h: "+h.print_expression()+" and i: "+i.print_expression()+"\n", h.equivalent(i));
        CPPUNIT_ASSERT_MESSAGE( "root qnode flip, should be equivalent. found c: "+c.print_expression()+" and j: "+j.print_expression()+"\n", c.equivalent(j));
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
    
    void testLeafList() //tests adding leaves to ensure they add to the list you pass in correctly
    {
        std::list<Leaf*> lfs;
        for(int i=0; i<5; i++){
            new Leaf(i, lfs);
        }
        size_t count = lfs.size();
        CPPUNIT_ASSERT_EQUAL(count, (size_t)5);
        for(auto it= lfs.begin(); it!=lfs.end(); ++it){
            delete *it;
            *it = NULL;
        }
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
    CPPUNIT_TEST( test_marking ); 
    CPPUNIT_TEST( test_reduce_qroot );
    CPPUNIT_TEST( test_reduce_proot );
	CPPUNIT_TEST_SUITE_END();

public:
    
    void test_marking()
    {
        //mock a node with one partial child and two full and two empty
        std::vector<int> v = {2, 2, 3, 4};
        std::list<Leaf*> lst;
        PQnode *root = new PQnode(v, lst); // {2, 2, 3, 4}
        PQnode *child = new PQnode(v, lst); // {2, 2, 3, 4}
        root->link_child(child); // {2, 2, 3, 4 {2, 2, 3, 4} }
        
        for(std::list<Leaf*>::iterator itr = lst.begin(); itr != lst.end(); ++itr) //mark the value = 2 children as full
        {
            if((*itr)->get_value()==2){
                (*itr)->mark();
            }
        }
        
        child->mark(); //mark the child
        root->mark(); //mark the root
        
        CPPUNIT_ASSERT_EQUAL(child->get_mark(), partial);
        CPPUNIT_ASSERT_EQUAL(root->get_mark(), partial);
        
        delete root;
    }
    
    void test_reduce_qroot()
    {
        std::vector<int> v = {5, 2, 3, 1};
        std::list<Leaf*> lst;
        PQnode *root = new PQnode(v, lst); // {5, 2, 3, 1}
        std::vector<int> vv = {2, 1, 3, 4};
        PQnode *child = new PQnode(vv, lst); // {2, 1, 3, 4}
        root->link_child(child); // {5, 2, 3, 1 {2, 1, 3, 4} }
        root->set_type(qnode); // [5, 2, 3, 1 {2, 1, 3, 4} ]
                
        for(std::list<Leaf*>::iterator itr = lst.begin(); itr != lst.end(); ++itr) //mark the value = 2 children as full
        {
            if((*itr)->get_value()==1){
                (*itr)->mark();
            }
        }
        
        child->mark(); //mark the child
        root->mark(); //mark the root
        root->reduce(); //apply the reduction
        
        std::string expected = "[ 5  2  3  1  1  { 2  3  4 } ]";
        std::string message = "expected [ 5  2  3  1  1  { 2  3  4 } ] but found "+root->print_expression()+"\n";
        CPPUNIT_ASSERT_MESSAGE( message, custom::compare(expected, root->print_expression()));
        
        delete root;
    }
    
    void test_reduce_proot()
    {
        std::vector<int> v = {5, 2, 3, 1};
        std::list<Leaf*> lst;
        PQnode *root = new PQnode(v, lst); // {5, 2, 3, 1}
        std::vector<int> vv = {2, 1, 3, 4};
        PQnode *child = new PQnode(vv, lst); // {2, 1, 3, 4}
        root->link_child(child); // {5, 2, 3, 1 {2, 1, 3, 4} }
                
        for(std::list<Leaf*>::iterator itr = lst.begin(); itr != lst.end(); ++itr) //mark the value = 2 children as full
        {
            if((*itr)->get_value()==1){
                (*itr)->mark();
            }
        }
        
        child->mark(); //mark the child
        root->mark(); //mark the root
        root->reduce(); //apply the reduction
        
        std::string expected = "{ 5  2  3  [ { 2  3  4 }  1  1 ] }";
        std::string message = "expected "+expected+" but found "+root->print_expression()+"\n";
        CPPUNIT_ASSERT_MESSAGE( message, custom::compare(expected, root->print_expression()));
        
        delete root;
    }
};

int main( int argc, char **argv)
{
    
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( PQTreeTests::suite() );
    runner.addTest( PQnodeTests::suite() );
    runner.addTest( LeafTests::suite() );
    runner.run();
    
    //PQTree tree("{ 3 4 5 [ 3 4 5 [ 3 2 1 7 ]] { 8 9 10 }}");
    //std::cout << "the potentional gml file is \n\n" << tree.convert_to_gml() << std::endl;
    
    return 0;
}


