#ifndef NETDEV_H_
#define NETDEV_H_
#include "syshead.h"
#include "ethernet.h"

/// @brief  设备地址 ip + mac
struct netdev{
    uint32_t addr;              // ip
    unsigned char hwaddr[6];    // mac
};

void network_device_init(struct netdev *dev, char *addr, char *hwaddr);

void netdev_transmit(struct netdev *dev, struct eth_hdr *hdr, uint16_t ethernet_type, int len, unsigned char *dst);

#endif