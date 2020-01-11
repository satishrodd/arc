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
 * The function which is to print the node information.
 * Will be used as callback for tree walks.
 */
void 
print_node(node_ptr n) {
    printf("> %s/%d\n",
           prefix_str(n->prefix.s_addr),
           n->len);
}

/*
 * Entry for the program.
 */
void main() 
{
    u_int32_t operation, len, num;
    struct in_addr ip, mask;
    ret_types ret;

    node_ptr root = NULL;
    node_ptr n= NULL;

    do {
        printf("\n1.Add 2.Delete 3.longest_match 4.subtree walk 0.exit\n");
        scanf("%d", &operation);
        switch(operation) {
          case EXIT:
            return;
          case ADD:
            if ( !root ) {
                root = tree_init();
            }
            printf("enter the number of entries\n");
            scanf("%d",&num);
            while(num) {
                get_ip(&ip);
                printf("subnet mask len: ");
                scanf("%d", &len);
                mask.s_addr = GET_MASK(len);
                ret = node_insert(root, ip.s_addr, mask.s_addr);
                if ( ret == FAIL ) {
                    PRINT_ERROR("Insertion failed\n");
                }
                num--;
            }
            break;
          case DEL:
            get_ip(&ip);
            printf("subnet mask len: ");
            scanf("%d", &len);
            mask.s_addr = GET_MASK(len);
            ret = node_delete(root, ip.s_addr, mask.s_addr);
            if ( ret == FAIL ) {
                PRINT_ERROR("Deletion failed\n");
            }
            break;
          case SUB_WALK:
            get_ip(&ip);
            printf("subnet mask len: ");
            scanf("%d", &len);
            printf("\n");
            mask.s_addr = GET_MASK(len);
            ret = subtree_walk(root, ip.s_addr, mask.s_addr,
                               print_node);
            if ( ret == FAIL ) {
                PRINT_ERROR("SUBWALK failed\n");
            }
            break;
          case MATCH:
            get_ip(&ip);
            n = longest_prefix_match(root, ip.s_addr);
            if (!n) {
                PRINT_ERROR("Longest prefix match failed.\n");
                break;
            }
            printf("\nLongest Prefix found is: ");
            print_node(n);
            break;
          default:
            printf("Invalid selection.\n");
            return;
        }
    } while(operation);
}
