#ifndef ETHERNET_H_
#define ETHERNET_H_
#include <stdint.h>
#include <linux/if_ether.h>

/// @brief Ethernet Header Format
struct eth_hdr
{   
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t eth_type;
    unsigned char payload[];

}__attribute__((packed));


struct ehc_hdr* init_ethernet_header(char *buf);

#endif