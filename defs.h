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

#define ZERO        0
#define ONE         1
#define MAX_CHILDS  2
#define DEBUG_ENABLED true

#define PRINT_ERROR(format, ...) \
  printf("\n[%s:%d] ERROR:", __func__, __LINE__);\
  printf(format, ##__VA_ARGS__);

#define PRINT_DEBUG(format, ...) \
  if(DEBUG_ENABLED) { \
      printf("\n[%s:%d] ", __func__, __LINE__);\
      printf(format, ##__VA_ARGS__);\
  }\

typedef struct node *node_ptr;

struct node {
    node_ptr    children[MAX_CHILDS];
    bool        leaf;
};

int NODE_SIZE = sizeof(struct node);


#endif /* DEFS_H */
