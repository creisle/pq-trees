

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */
#include "PQTree.h"
#include <stdlib.h>
#include <sstream>

#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wpadded"




int main( int argc, char **argv)
{
    
    
    PQTree tree("{ 3 4 5 [ 3 4 5 [ 3 2 1 7 ]] { 8 9 10 }}");
    //std::cout << "the potentional gml file is \n\n" << tree.convert_to_gml() << std::endl;
    
    return 0;
}


