#include "ipv4.h"
#include "utils.h"

void ipv4_incoming(struct netdev *dev, struct eth_hdr *ethhdr){
    struct iphdr *tmp_iphdr = (struct iphdr*)ethhdr->payload;

    uint16_t csum = -1;

    // 这里的version ihl ttl 都是小于等于 1个字节的数据 所以不需要 ntohs
    if(tmp_iphdr->version != LNET_IPV4_VERSION){
        _utils_print_error("ip datagram version is not ipv4.\n");
    }

    if(tmp_iphdr->ihl < 5){
        _utils_print_error("ipv4 header length must be at least 5.\n");
    }

    if(tmp_iphdr->ttl == 0){
        _utils_print_error("time to live of ip datagram reached 0.\n");
    }

    csum = _utils_check_sum(tmp_iphdr, tmp_iphdr->ihl * 4);             // * 4个字节



}

void ipv4_outgoing(struct netdev *dev, struct eth_hdr *ethhdr){

}