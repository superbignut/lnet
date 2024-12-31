#include "tcp.h"
#include "utils.h"

/// @brief 
/// @param dev 
/// @param hdr 
void tcp_in(struct netdev *dev, struct eth_hdr *hdr){
    struct iphdr *iphdr = (struct iphdr*)hdr->payload;
    struct tcphdr *tcphdr = (struct tcphdr *)iphdr->data;

    if(tcp_checksum(iphdr, tcphdr) != 0){
        _utils_print_error("tcp checksum did not match, dropping!\n");
        return;
    }
    tcp_out(dev, hdr);
}

/// @brief  tcp 回复
/// @param dev 
/// @param hdr 
void tcp_out(struct netdev *dev, struct eth_hdr *hdr){


}

/// @brief 计算 tcp 校验和
/// @param iphdr 
/// @param tcphdr 
/// @return 
int tcp_checksum(struct iphdr *iphdr, struct tcphdr *tcphdr){
    struct tcpiphdr pseudo_hdr;
}
