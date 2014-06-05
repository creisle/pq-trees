#include "PQTree.h"
#include <cpptest.h>

// warnings to tell the compiler to ignore
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wc++98-compat"

class LeafTests : public Test::Suite{
    public:
        LeafTests(){
            TEST_ADD(LeafTests::first_test)
            TEST_ADD(LeafTests::second_test)
        }
        
    private:
        void first_test();
        void second_test();
};

class PQTreeTests : public Test::Suite{
    public:
        PQTreeTests(){
            TEST_ADD(PQTreeTests::first_test)
            TEST_ADD(PQTreeTests::second_test)
        }
        
    private:
        void first_test();
        void second_test();
};

class PQnodeTests : public Test::Suite{
    public:
        PQnodeTests(){
            TEST_ADD(PQnodeTests::first_test)
            TEST_ADD(PQnodeTests::second_test)
        }
        
    private:
        void first_test();
        void second_test();
};