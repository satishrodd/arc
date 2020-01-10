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
//#include <netinet/in.h>

#define ZERO            0
#define ONE             1
#define MAX_CHILDS      2
#define DEBUG_ENABLED   true
#define FIRST_BIT       0x80000000
#define MAX_LEN         32

typedef enum {
    FAIL,
    PASS
} ret_types;

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

typedef void (*callback_func)(node_ptr);

struct node {
    u_int32_t   prefix;
    node_ptr    children[MAX_CHILDS];
    node_ptr    parent;
    bool        leaf;
    u_int8_t    len;
};

int NODE_SIZE = sizeof(struct node);

/*
 * Explicit declaration of some functions.
 */
char *inet_ntoa(u_int32_t addr);
#endif /* DEFS_H */
