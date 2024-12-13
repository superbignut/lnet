#ifndef ETHERNET_H_
#define ETHERNET_H_
#include <stdint.h>
#include <linux/if_ether.h>

/// @brief Ethernet Header Format
struct eth_hdr
{   
    uint8_t dmac[6];
    uint8_t smac[6];
    uint16_t eth_type;
    uint8_t payload[];

}__attribute__((packed));


struct eth_hdr* init_ethernet_header(char *buf);

#endif