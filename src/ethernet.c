#include "ethernet.h"
#include "syshead.h"


struct ehc_hdr* init_eth_hdr(char * buf){

    struct eth_hdr * hdr = (struct eth_hdr *) buf;
    
    hdr->ether_type = htons(hdr->ether_type);

    return hdr;

}