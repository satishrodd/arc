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
#include <netinet/in.h>

/*
 * Allocate the memory for the node and
 * initialize it.
 *
 * Return NULL upon failure, else pointer to memory.
 */
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
    n->leaf             = false;
    n->children[ZERO]   = NULL;
    n->children[ONE]    = NULL;
    n->prefix           = 0;
    n->len              = 0;
    PRINT_DEBUG("Success[%p]", n);
    return(n);
}

/*
 * Insert the node in the prefix_tree
 * return PASS upon success, FAIL upon failure.
 */
ret_types
node_insert(node_ptr    root,
            u_int32_t   prefix,
            u_int32_t   mask) {
    node_ptr curr   = root;
    node_ptr new    = NULL;
    u_int8_t index  = 0, depth = 0;
    u_int32_t  tmp_prefix   = prefix;//, mask  = 0;

    if ( !root ) {
        PRINT_ERROR("Tree root is NULL");
        return(FAIL);
    }
    
    for (;mask;) {
        depth++;
        index = (prefix & FIRST_BIT)? 1:0;
        /*
         * If the child does not exist, but len is
         * not exausted, we need to create the node.
         */
        if ( !curr->children[index] ) {
            new = node_alloc(); 
            if (!new) {
                return(FAIL);
            }
            /*
             * Every node created, will have the prefix
             * from root to itself, stored in it, along with the
             * depth from root, as len;
             */
            new->prefix = (tmp_prefix & GET_MASK(depth));
            new->len    = depth;
            new->parent = curr;
            curr->children[index] = new;
            curr = new;
        } else {
            curr = curr->children[index];
        }
        SHIFT_LEFT(prefix);
        SHIFT_LEFT(mask);
    }
    /*
     * Once the complete prefix walk is done, 
     * then we must have found the node, and need to mark it 
     * as leaf.
     */
    PRINT_DEBUG("Inserted prefix:%s\n", 
                (char *)inet_ntoa(htonl(curr->prefix)));
    curr->leaf = true;
    return(PASS);
}

ret_types
node_delete(node_ptr    root,
            u_int32_t   prefix,
            u_int32_t   mask) {
    node_ptr curr   = root;
    node_ptr parent = NULL;
    u_int8_t index  = 0, depth = 0;
    u_int32_t  tmp_prefix   = prefix;//, mask  = 0;

    if ( !root ) {
        PRINT_ERROR("Tree root is NULL");
        return(FAIL);
    }
    
    for (;mask;) {
        index = (prefix & FIRST_BIT)? 1:0;
        /*
         * If the child does not exist, but len is
         * not exausted, we need to create the node.
         */
        if ( !curr->children[index] ) {
            PRINT_ERROR("Prefix not found\n");
            return(PASS);
        } else {
            curr = curr->children[index];
        }
        SHIFT_LEFT(prefix);
        SHIFT_LEFT(mask);
    }
    /*
     * Clear leaf flag for node.
     * Note: it can still be an intermediate node, for 
     * a child.
     */
    curr->leaf = false;
    parent = curr->parent;
    /*
     * Free the node memory.
     * Also we need to free any intermediate nodes
     * which are not leaves, or do not have any children.
     */
    while (!curr->leaf &&
           !curr->children[ZERO] &&
           !curr->children[ONE]) {
           PRINT_DEBUG("Freeing node %p\n",curr); 
           free(curr);
           curr = parent;
           curr->children[index] = NULL;
           parent = parent->parent;
    }
    PRINT_DEBUG("deleted prefix:%s\n", 
                (char *)inet_ntoa(htonl(tmp_prefix)));
    return(PASS);
}

node_ptr
tree_init() {
    node_ptr n = NULL;
    n = node_alloc();
    if(n) {
        PRINT_DEBUG("success");
    }
    return(n);
}

void main() {
    node_ptr root = NULL;
    root = tree_init();
    node_insert(root, 0xa0000000, 0xf0000000); 
    node_insert(root, 0xb0000000, 0xf0000000); 
    node_insert(root, 0xc0000000, 0xf0000000); 
    node_insert(root, 0x70000000, 0xf0000000); 
    node_insert(root, 0x40000000, 0xf0000000); 
    node_delete(root, 0xc0000000, 0xf0000000); 
    node_delete(root, 0x70000000, 0xf0000000); 
}
