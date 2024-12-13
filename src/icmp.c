#include "icmp.h"
#include "ipv4.h"
#include "ethernet.h"
#include "utils.h"

/// @brief ip 中收到的是icmp 协议的处理函数, 对不同的type 进行分发
/// @param dev 
/// @param ethhdr 
void icmpv4_incoming(struct netdev *dev, struct eth_hdr *ethhdr){

    //  拿到ip包
    struct iphdr *tmp_iphdr = (struct iphdr*)ethhdr->payload;
    //  拿到 icmp包
    struct icmp_v4 *tmp_icmp_hdr = (struct icmp_v4*)tmp_iphdr->data;

    //  判断是哪个 icmp 进行分发
    switch (tmp_icmp_hdr->type)
    {
    case LNET_ICMP_TYPE_ECHO:
        icmpv4_reply(dev, ethhdr);
        break;
    default:
        _utils_print_error("icmp type did not match any supported types");
        break;
    }
}

/// @brief icmpv4 回复 echo_reply 报文
/// @param dev 
/// @param ethhdr 
void icmpv4_reply(struct netdev *dev, struct eth_hdr *ethhdr){
    
    //  拿到ip包
    struct iphdr *tmp_iphdr = (struct iphdr*)ethhdr->payload;
    //  拿到 icmp包
    struct icmp_v4 *tmp_icmp_hdr = (struct icmp_v4*)tmp_iphdr->data;

    //  所有 icmp 报文都加进校验和, len字段是16位 所以需要转换一下
    uint16_t icmp_len = ntohs(tmp_iphdr->len) - (tmp_iphdr->ihl * 4);  

    //  修改收到的类型 位 echo reply
    tmp_icmp_hdr->type = LNET_ICMP_TYPE_ECHO_REPLY;
    
    // 修改校验和
    tmp_icmp_hdr->csum = 0;
    tmp_icmp_hdr->csum = _utils_check_sum(tmp_icmp_hdr, icmp_len);

    //  ip包发回去
    ipv4_outgoing(dev, ethhdr);
}