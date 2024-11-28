#ifndef NETDEV_H_
#define NETDEV_H_
#include "syshead.h"
#include "ethernet.h"
struct netdev{
    uint32_t addr;
    unsigned char hwaddr[6];
};

void netdev_init(struct netdev *dev, char *addr, char *hwaddr);

void netdev_transmit(struct netdev *dev, struct eth_hdr *hdr, uint16_t ethernet_type, int len, unsigned char *dst);

#endif