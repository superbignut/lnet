#include "ipv4.h"
#include "utils.h"
#include "icmp.h"
#include "netdev.h"

/// @brief 以太网收到 ipv4 数据包， 处理函数
/// @param dev 
/// @param ethhdr 
void ipv4_incoming(struct netdev *dev, struct eth_hdr *ethhdr){
    
    struct iphdr *tmp_iphdr = (struct iphdr*)ethhdr->payload;

    uint16_t csum = -1;

    // ! 这里的version ihl ttl protocol 都是小于等于 1个字节的数据 所以不需要 ntohs
    if(tmp_iphdr->version != LNET_IPV4_VERSION){
        //  版本是 ipv4
        _utils_print_error("ip datagram version is not ipv4.\n");
        return;
    }

    if(tmp_iphdr->ihl < 5){
        //  首部长度是 5 * 4B
        _utils_print_error("ipv4 header length must be at least 5.\n");
        return;
    }

    if(tmp_iphdr->ttl == 0){
        //  ttl != 0
        _utils_print_error("time to live of ip datagram reached 0.\n");
        return;
    }

    //  4个字节 计算校验和
    csum = _utils_check_sum(tmp_iphdr, tmp_iphdr->ihl * 4);             

    if(csum != 0){
        //  校验和失败要丢弃
        _utils_print_error("csum is not error. droped.\n");
        return; 
    }

    switch (tmp_iphdr->protocol)
    {
        //  根据 protocol 字段分发给不同函数处理
    case LNET_IP_PROTOCOL_ICMPV4:
        //  分发给 icmp 进行处理
        icmpv4_incoming(dev, ethhdr);
        break;
    default:
        _utils_print_error("unknown ip protocol");
        break;
    }
}

/// @brief 
/// @param dev 
/// @param ethhdr 
void ipv4_outgoing(struct netdev *dev, struct eth_hdr *ethhdr){
    //  
    struct iphdr *tmp_iphdr = (struct iphdr*)ethhdr->payload;

    uint32_t tmp_addr;
    
    //  交换地址
    tmp_addr = tmp_iphdr->daddr;
    tmp_iphdr->daddr = tmp_iphdr->saddr;
    tmp_iphdr->saddr = tmp_addr;

    
    tmp_iphdr->csum = 0;
    tmp_iphdr->csum = _utils_check_sum(tmp_iphdr, tmp_iphdr->ihl * 4);
    netdev_transmit(dev, ethhdr, ETH_P_IP, ntohs(tmp_iphdr->len), ethhdr->smac);
}