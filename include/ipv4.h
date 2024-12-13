#ifndef IPV4_H_
#define IPv4_H_
#include "syshead.h"
#include "netdev.h"
#include "ethernet.h"

/*
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Version|  IHL  |Type of Service|          Total Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Identification        |Flags|      Fragment Offset    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Time to Live |    Protocol   |         Header Checksum       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       Source Address                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Destination Address                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Options                    |    Padding    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/*
+ + + + + + + + + + + 
  ihl       version 
+ + + + + + + + + + +
  0 1 2 3   4 5 6 7 + 
*/



struct iphdr{
    uint8_t ihl: 4;                 // 位域先放低地址位 = 5;
    uint8_t version : 4;            // 发送先发高地址位 = 4; // 所以这两位颠倒了一下 合起来是0x45
    uint8_t tos;                    // type of service 不使用
    uint16_t len;                   // len = len_header + len_data  以字节为单位 当前ip数据报的总长度
    uint16_t id;                    // 同一个分片后的 ip数据报 使用相同的 id
    uint16_t flag : 3;              // 最低为 MF 中间位 DF; MF=0 最后一个分片，MF=1 后面还有分片；DF=1进制分片；
    uint16_t frag_offset : 13;      // 8字节位单位，分片在原ip报的相对位置
    uint8_t ttl;                    // 跳数， 减到 0 不参与转发
    uint8_t protocol;               // 携带的是什么协议 ：ICMP IPV4...
    uint16_t csum;                  // 首部校验和
    uint32_t saddr;                 // 源ip
    uint32_t daddr;                 // 目的ip
    uint8_t data[];                 
}__attribute__((packed));


#define LNET_IPV4_VERSION 4


void ipv4_incoming(struct netdev *dev, struct eth_hdr *ethhdr);

void ipv4_outgoing(struct netdev *dev, struct eth_hdr *ethhdr);

#endif