#ifndef ETHERNET_H_
#define ETHERNET_H_
#include <stdint.h>
#include <linux/if_ether.h>

struct eth_hdr
{   
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ether_type;
    unsigned char payload[];

}__attribute__((packed));


/**
 * @brief Init the ethernet header.
 * 
 * @param buf 
 * @return struct ehc_hdr* 
 */
struct ehc_hdr* init_eth_hdr(char * buf);

#endif