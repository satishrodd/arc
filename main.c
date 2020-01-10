/*---------------------------------------------------------------------------
 * 
 * main.c 
 *
 *     01/10/2020 - 
 *
 *     < rodd.satish@gmail.com >
 *
 *
 *
 *---------------------------------------------------------------------------
 */

#include"defs.h"
#include<stdlib.h>

node_ptr 
node_alloc() {
    node_ptr n = NULL;
    // Allocate the memory.
    n = malloc(NODE_SIZE);
    if ( !n ) {
        PRINT_ERROR("Malloc failed.");
        return(NULL);
    }
    //Initialize the node.
    n->leaf = false;
    n->children[ZERO] = NULL;
    n->children[ONE]  = NULL;
    PRINT_DEBUG("Success[%p]", n);
    return(n);
}

void main() {

    node_ptr n1;
    n1 = node_alloc();

}
