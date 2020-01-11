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
    n->prefix.s_addr    = 0;
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

    if ( !prefix || !mask  ) {
        PRINT_ERROR("Invalid prefix/mask len");
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
            new->prefix.s_addr = (tmp_prefix & GET_MASK(depth));
            //new->prefix.s_addr = tmp_prefix;
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
                print_prefix(curr->prefix.s_addr));
    curr->leaf = true;
    return(PASS);
}

/*
 * Deletes the prefix node.
 * Also deletes the intermediate nodes, which are not leaves
 * nor parents.
 *
 * returns:
 *  FAIL: when node not found
 *  PASS: when the node successfully deleted.
 */
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
                print_prefix(tmp_prefix));
    return(PASS);
}

/*
 * This function will search the tree and look for the
 * longest prefix that is matched by the given prefix.
 */
node_ptr
longest_prefix_match(node_ptr root,
                     u_int32_t prefix) 
{
    node_ptr   curr           = root;
    node_ptr   longest_prefix = NULL;
    u_int32_t  tmp_prefix     = prefix;//, mask  = 0;
    u_int8_t   index  = 0, depth = 0;

    if ( !root ) {
        PRINT_ERROR("Tree root is NULL");
        return(FAIL);
    }
    
    for (int i=0;i<=MAX_LEN;i++) {
        index = (prefix & FIRST_BIT)? 1:0;

        /*
         * If the current node is the leafe node,
         * store that entry in the temporary pointer.
         */
        if ( curr->leaf ) {
            longest_prefix = curr;
        }
        /*
         * If the child does not exist, but len is
         * not exausted, we need to create the node.
         */
        if ( !curr->children[index] ) {
            return(longest_prefix);
        } else {
            curr = curr->children[index];
        }
        SHIFT_LEFT(prefix);
    }
   return(longest_prefix); 
}
/*
 * The function which is to print the node information.
 * Will be used as callback for tree walks.
 */
void print_node(node_ptr n) {
    printf("%s/%d\n",
           print_prefix(n->prefix.s_addr),
           n->len);
}

/*
 * Walks the tree in post order form, and passes each 
 * node to the callback function taken as argument.
 */
ret_types
tree_walk (node_ptr root,
           callback_func fn) {
    if ( !root ) {
        return (PASS);
    }
    if ( root->leaf  ) {
        fn(root);
    }
    tree_walk(root->children[ZERO], fn);
    tree_walk(root->children[ONE], fn);
}

/*
 * Walks all the nodes under a node identified by 
 * the prefix and mask given as argument.
 */
ret_types
subtree_walk(node_ptr  root,
             u_int32_t prefix,
             u_int32_t mask,
             callback_func fn) {

    node_ptr curr   = root;
    u_int8_t index  = 0;

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
            return(FAIL);
        } else {
            curr = curr->children[index];
        }
        SHIFT_LEFT(prefix);
        SHIFT_LEFT(mask);
    }
    /*
     * After identifying the node of the given prefix and 
     * subnet mask, call the tree walk function by passing the
     * found node as root.
     */
    return(tree_walk(curr, fn));
}

/*
 * Initialize the root node.
 */
node_ptr
tree_init() {
    node_ptr n = NULL;
    n = node_alloc();
    if(n) {
        PRINT_DEBUG("success");
    }
    return(n);
}

void 
get_ip(struct in_addr *ip)
{
    char ipaddr[15];
    printf("ip:");
    scanf("%s", ipaddr);
    inet_aton(ipaddr, ip);
    ip->s_addr = htonl(ip->s_addr);;
    return;
}

/*
 * Entry for the program.
 */
void main() {
    u_int32_t operation, len, num;
    struct in_addr *ip, *mask;
    ret_types ret;

    node_ptr root = NULL;
    node_ptr n= NULL;
    ip      = malloc(sizeof(struct in_addr));
    mask    = malloc(sizeof(struct in_addr));

    root = tree_init();
    do {
        printf("1.Add 2.Delete 3.longest_match 4.subtree walk 0.exit\n");
        scanf("%d", &operation);
        switch(operation) {
          case EXIT:
            return;
          case ADD:
            printf("enter the number of entries\n");
            scanf("%d",&num);
            printf("enter prefix and subnet mask length\n");
            while(num) {
                get_ip(ip);
                printf("subnet mask len: ");
                scanf("%d", &len);
                mask->s_addr = GET_MASK(len);
                ret = node_insert(root, ip->s_addr, mask->s_addr);
                if ( ret == FAIL ) {
                    printf("\nInsertion failed");
                }
                num--;
            }
            break;
          case DEL:
            printf("enter prefix and subnet mask length\n");
            get_ip(ip);
            printf("subnet mask len: ");
            scanf("%d", &len);
            mask->s_addr = GET_MASK(len);
            ret = node_delete(root, ip->s_addr, mask->s_addr);
            if ( ret == FAIL ) {
                printf("\nDeletion failed");
            }
            break;
          case SUB_WALK:
            printf("enter prefix and subnet mask length\n");
            get_ip(ip);
            printf("subnet mask len: ");
            scanf("%d", &len);
            mask->s_addr = GET_MASK(len);
            ret = subtree_walk(root, ip->s_addr, mask->s_addr,
                               print_node);
            if ( ret == FAIL ) {
                printf("\nSUBWALK failed");
            }
            break;
          case MATCH:
            printf("enter prefix\n");
            get_ip(ip);
            n = longest_prefix_match(root, ip->s_addr);
            if (!n) {
                printf("\nLongest prefix match failed.");
                break;
            }
            print_node(n);
            break;
          default:
            printf("Invalid selection.\n");
            return;
        }
    } while(operation);
}
