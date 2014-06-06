

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include "PQTree.h"

class ExampleTests : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ExampleTests );
	CPPUNIT_TEST( example );
	CPPUNIT_TEST( anotherExample );
	CPPUNIT_TEST( testEquals );
	CPPUNIT_TEST_SUITE_END();

	double m_value1;
	double m_value2;

public:
	void example()
	{
	  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 1.1, 0.05 );
	  CPPUNIT_ASSERT( 1 == 0 );
	  CPPUNIT_ASSERT( 1 == 1 );
	}

	void anotherExample()
	{
        //std::vector<int> v = {1, 2, 3};
        //PQTree::PQTree tree(v);
        int n = 5;
        for(int i=0; i<n; i++){
            Leaf *lf = new Leaf(i);
        }
        int count = 6;
        
        CPPUNIT_ASSERT_EQUAL(n, count);
	}

	void setUp()
	{
	  m_value1 = 2.0;
	  m_value2 = 3.0;
	}
    
    void tearDown()
    {
    }


	void testEquals()
	{
	  long* l2 = new long(12);
      long* l1 = new long(11);

	  CPPUNIT_ASSERT_EQUAL( 12, 12 );
	  CPPUNIT_ASSERT_EQUAL( 12L, 12L );
	  CPPUNIT_ASSERT_EQUAL( *l1, *l2 );

	  delete l1;
	  delete l2;

	  CPPUNIT_ASSERT( 12L == 12L );
	  CPPUNIT_ASSERT_EQUAL( 12, 13 );
	  CPPUNIT_ASSERT_DOUBLES_EQUAL( 12.0, 11.99, 0.5 );
	}
};

int main( int argc, char **argv)
{
    std::vector<int> v = {1, 2, 3};
    PQTree::PQTree tree(v);
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( ExampleTests::suite() );
    runner.run();
    return 0;
}
