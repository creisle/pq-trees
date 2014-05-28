/*
PQ trees implementation
ref: booth and lueker, 1976
ref: sarah young, 1977

a first attempt at implementing pq tree for testing graph planarity

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define some constants

#define TRUE        1
#define FALSE       0
#define MAX_CHILD   10

/*
 * 1. create p-nodes, q-nodes and c-nodes
 * 2. create reduction templates
 * 3. ...
 *
 *
 *
 *
 *
 */

struct node {
    struct node *parent;
    //currently this is an array. will later make a list....
    int children[MAX_CHILD];
    int value;
};

//safe malloc function
void* emalloc(int n){
    void *p;
    p = malloc(n);
    if(p==NULL){
        fprintf(stderr, "malloc of %u bytes failed", n);
        exit(1);
    }
    return p;
}

new_node(){
    
}

int main(){
    printf("start of program\n");
    return 0;
}