/*---------------------------------------------------------------------------
 * 
 * defs.h 
 *
 *     01/10/2020 - 
 *
 *     < rodd.satish@gmail.com >
 *
 *
 *
 *---------------------------------------------------------------------------
 */

#ifndef DEFS_H
#define DEFS_H

#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include <netinet/in.h>

#define ZERO            0
#define ONE             1
#define MAX_CHILDS      2
#define DEBUG_ENABLED   false
#define FIRST_BIT       0x80000000
#define MAX_LEN         32
/*
 * Return types used by prefix_tree functions.
 */
typedef enum {
    FAIL,
    PASS
} ret_types;

/*
 * Operations for the user.
 */
enum {
    EXIT = 0,
    ADD,
    DEL,
    MATCH,
    SUB_WALK
};

#define PRINT_ERROR(format, ...) \
  printf("\n[%s:%d] ERROR:", __func__, __LINE__);\
  printf(format, ##__VA_ARGS__);

#define PRINT_DEBUG(format, ...) \
  if(DEBUG_ENABLED) { \
      printf("\n[%s:%d] ", __func__, __LINE__);\
      printf(format, ##__VA_ARGS__);\
  }\

#define SHIFT_LEFT(prefix) (prefix = prefix<<1)

#define GET_MASK(len) \
  (0xffffffff <<(MAX_LEN-len))

typedef struct node *node_ptr;

/*
 * typedef for the callback function which will be called
 * in tree walks.
 */
typedef void (*callback_func)(node_ptr);

/*
 * main structure for the prefix tree.
 * It will hold, all the information of a prefix. 
 */
struct node {
    struct in_addr  prefix;                 //IP address
    node_ptr        children[MAX_CHILDS];   // list of Children
    node_ptr        parent;                 // pointer to parent
    bool            leaf;                   
    u_int8_t        len;                    // ip subnet mask length
};



/*
 * Function declarations
 */

char *
prefix_str(u_int32_t prefix);

ret_types
node_insert(node_ptr    root,
            u_int32_t   prefix,
            u_int32_t   mask);

ret_types
node_delete(node_ptr    root,
            u_int32_t   prefix,
            u_int32_t   mask);

ret_types
subtree_walk(node_ptr  root,
             u_int32_t prefix,
             u_int32_t mask,
             callback_func fn);

node_ptr
longest_prefix_match(node_ptr root,
                     u_int32_t prefix);

node_ptr tree_init();

#endif /* DEFS_H */
