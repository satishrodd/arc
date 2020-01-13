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
#include<string.h>
#include <netinet/in.h>

ret_types 
get_ip(struct in_addr *ip)
{
    char ipaddr[15];
    printf("ip:");
    scanf("%s", ipaddr);
    if (!is_valid_ip(ipaddr) ) {
        return FAIL;
    }
    inet_aton(ipaddr, ip);
    ip->s_addr = htonl(ip->s_addr);;
    return PASS;
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
    u_int32_t operation=0, len=0, num=0;
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
            printf("enter the number of entries(max 100)\n");
            scanf("%d",&num);
            if ( num >100 ) {
                printf("Invalid entry\n");
                return;
            }
            while(num) {
                if(!get_ip(&ip)) {
                    printf("Invalid IP entered\n");
                    continue;
                }
                printf("subnet mask len: ");
                scanf("%d", &len);
                if ( len>MAX_LEN ) {
                    printf("invalid subnet mask len \n");
                    continue;
                }
                mask.s_addr = GET_MASK(len);
                ret = node_insert(root, ip.s_addr, mask.s_addr);
                if ( ret == FAIL ) {
                    PRINT_ERROR("Insertion failed\n");
                }
                num--;
            }
            break;
          case DEL:
            if(!get_ip(&ip)) {
                printf("Invalid IP entered\n");
                continue;
            }
            printf("subnet mask len: ");
            scanf("%d", &len);
            if ( len>MAX_LEN ) {
                printf("invalid subnet mask len \n");
                continue;
            }
            mask.s_addr = GET_MASK(len);
            ret = node_delete(root, ip.s_addr, mask.s_addr);
            if ( ret == FAIL ) {
                PRINT_ERROR("Deletion failed\n");
            }
            if (root->children[ZERO] && !root->children[ONE]) {
                free(root);
                root = NULL;
            }
            break;
          case SUB_WALK:
            if(!get_ip(&ip)) {
                printf("Invalid IP entered\n");
                continue;
            }
            printf("subnet mask len: ");
            scanf("%d", &len);
            if ( len>MAX_LEN ) {
                printf("invalid subnet mask len \n");
                continue;
            }
            printf("\n");
            mask.s_addr = GET_MASK(len);
            ret = subtree_walk(root, ip.s_addr, mask.s_addr,
                               print_node);
            if ( ret == FAIL ) {
                PRINT_ERROR("SUBWALK failed\n");
            }
            break;
          case MATCH:
            if(!get_ip(&ip)) {
                printf("Invalid IP entered\n");
                continue;
            }
            n = longest_prefix_match(root, ip.s_addr);
            if (!n) {
                printf("Longest prefix not found.\n");
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

/*
 * Basic validation of the IP address entered.
 */
ret_types
is_valid_ip(char *ip) {
    char *ch;
    char set[12];
    int i=0, len=0, dc=0;
    ch = ip;

    len = strlen(ip);

    if(len>15 || len <7) {
        return FAIL;
    }

    while(1) {
        if(*ch=='.' || *ch=='\0') {
            if ( *ch=='.'  ) {
                dc++;
                if(dc>3)return FAIL;
            }
            if ( i == 0  ) {
                return FAIL;
            }
            set[i]='\0';
            if ((u_int32_t)atoi(set)>255) {
                return FAIL;
            }
            if (*ch=='\0') {
                return PASS;
            }
            i=0;
            ch++;
        }

        if ( *ch <'0' || *ch >'9' ) {
            return FAIL;
        }
        set[i++] = *ch;
        ch++;
    }
    return PASS;
}


