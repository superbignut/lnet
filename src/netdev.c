#include "syshead.h"
#include "netdev.h"
#include "utils.h"
#include "ethernet.h"
#include "arp.h"
#include "tuntap_if.h"

/// @brief 初始化 netdev 设置的 ip 的 mac 地址
/// @param dev 
/// @param addr 
/// @param hwaddr 
void network_device_init(struct netdev *dev, char *addr, char *hwaddr){
    
    _utils_clear_array(*dev);

    if(inet_pton(AF_INET, addr, &dev->addr) != 1){      // addr is written in network byte order.
        perror("parsing inet addr failed.");
        exit(1);
    }
    // 读取mac 地址
    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hwaddr[0], &dev->hwaddr[1],
                                                    &dev->hwaddr[2], &dev->hwaddr[3],
                                                    &dev->hwaddr[4], &dev->hwaddr[5]); // hhx means unsigned char.
}

/// @brief 发送 不同 ethernet_type 类型的以太网数据包, 这里暂时不清楚为什么dst要单独拿出来，是为了更多的泛化吗
/// @param dev 
/// @param hdr 
/// @param ethernet_type 输入小端
/// @param len 
/// @param dst 
void netdev_transmit(struct netdev *dev, struct eth_hdr *ethhdr, uint16_t ethernet_type, int len, uint8_t *dst){
    ethhdr->eth_type = htons(ethernet_type);
    
    memcpy(ethhdr->dmac, dst, LNET_ETH_ADDR_LEN);
    memcpy(ethhdr->smac, dev->hwaddr, LNET_ETH_ADDR_LEN);

    len += sizeof(struct eth_hdr);
    
    tun_tap_write((char*)ethhdr, len);
}
