#include "tcp.h"
#include "utils.h"

/// @brief tcp 输入 处理函数，这里的 checksum 时传入真实的 ip 即可， 因为是校验收到的报文
/// @param dev 
/// @param hdr 
void tcp_in(struct netdev *dev, struct eth_hdr *hdr){
    struct iphdr *iphdr = (struct iphdr *)hdr->payload;
    struct tcphdr *tcphdr = (struct tcphdr *)iphdr->data;
    
    if(tcp_checksum(iphdr, tcphdr) != 0){
        _utils_print_error("tcp checksum did not match, dropping!\n");
        return;
    }
    tcp_out(dev, hdr);
}

/// @brief  tcp 回复，这里比较关键的地方就是 中间 构造了一个 伪 ip  
/// @param dev 
/// @param hdr 
void tcp_out(struct netdev *dev, struct eth_hdr *hdr){
    struct iphdr *iphdr = (struct iphdr *)hdr->payload;
    struct tcphdr *tcphdr = (struct tcphdr *)iphdr->data;
    struct iphdr pseudo_ip_hdr;                             //  伪ip， 为了传给 tcp_checksum
    
    uint16_t tmp_port = tcphdr->dport;                      //  tcp 端口号交换
    tcphdr->dport = tcphdr->sport;
    tcphdr->sport = tmp_port;

    if(tcphdr->flags & LNET_TCP_SYN){                       //  
        tcphdr->flags |= LNET_TCP_ACK;                      //  ack
        tcphdr->ack = htonl(ntohl(tcphdr->seq) + 1);        //  确认字段+1 第一次握手没有数据，但要消耗一个序号
        tcphdr->seq = htonl(12345678);                      
    }
    // tcphdr->hl = 5;
    // iphdr->len -= LNET_TCP_HDR_LEN;                      //  ??????
    
                                                            //  构造发送端的伪ip, 注意不是伪首部 主要就是交换一下 ip 
    pseudo_ip_hdr.saddr = iphdr->daddr;
    pseudo_ip_hdr.daddr = iphdr->saddr;
    pseudo_ip_hdr.protocol = iphdr->protocol;
    pseudo_ip_hdr.len = iphdr->len;                         //  tcp 数据报的长度
    pseudo_ip_hdr.ihl = iphdr->ihl;                         //  

    tcphdr->csum = 0;
    tcphdr->csum = tcp_checksum(&pseudo_ip_hdr, tcphdr);
    ipv4_outgoing(dev, hdr);                                //  回复
}

/// @brief 计算 tcp 校验和：比较特别的地方是，tcp的校验和需要 ip 报文的部分内容，因此需要构造一个伪头部
///        需要注意的是，在发送报文的时候，由于这时的 ip报文的很多内容还没被修改（），因此 tcp_out 中则是伪造了一个 ip 传进来
/// @param iphdr 真实的或者构造的 ip 数据报，用来提取并构造 tcp 校验和 所需要的伪 ip 头信息
/// @param tcphdr 真实的 tcp 报文
/// @return 
int tcp_checksum(struct iphdr *iphdr, struct tcphdr *tcphdr){
    
    struct pseudo_ip_in_tcp_csum pseudo_hdr;               //  tcp 校验和构造伪首部
    uint32_t sum = 0;   

    int tlen = ntohs(iphdr->len) - iphdr->ihl * 4;          //  tcp 数据的总长度
    
    pseudo_hdr.saddr = iphdr->saddr;
    pseudo_hdr.daddr = iphdr->daddr;
    pseudo_hdr.zero = 0;
    pseudo_hdr.proto = iphdr->protocol;
    pseudo_hdr.tlen = htons(tlen);                          //  tcp长度 htons 而不是 htonl
    // printf("second %d \n", tlen);
    sum = sum_every_16bits(&pseudo_hdr, sizeof(struct pseudo_ip_in_tcp_csum));
    // printf("third %d %d \n", pseudo_hdr.tlen, tlen);
    return _utils_check_sum(tcphdr, tlen, sum);             //  在伪首部的基础上计算校验和
}
