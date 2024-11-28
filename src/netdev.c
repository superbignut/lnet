#include "syshead.h"
#include "netdev.h"
#include "utils.h"
#include "ethernet.h"

void netdev_init(struct netdev *dev, char *addr, char *hwaddr){
    _utils_clear_array(*dev);

    if(inet_pton(AF_INET, addr, &dev->addr) != 1){
        perror("parsing inet addr failed.");
        exit(1);
    }
    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hwaddr[0],
                                                    &dev->hwaddr[1],
                                                    &dev->hwaddr[2],
                                                    &dev->hwaddr[3],
                                                    &dev->hwaddr[4],
                                                    &dev->hwaddr[5]);
}

void netdev_transmit(struct netdev *dev, struct eth_hdr *hdr, uint16_t ethernet_type, int len, unsigned char *dst){
    hdr->ether_type = htons(ethernet_type);
    memcpy(hdr->smac, dev->hwaddr, 6);
    memcpy(hdr->dmac, dst, 6);
    len += sizeof(struct eth_hdr);
    
}
