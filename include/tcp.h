#ifndef TCP_H_
#define TCP_H_
#include "syshead.h"
#include "netdev.h"
#include "ethernet.h"
#include "ipv4.h"
/*

A TCP header, followed by any user data in the segment, is formatted as follows, using the style from [66]:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |          Source Port          |       Destination Port        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        Sequence Number                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Acknowledgment Number                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Data |       |C|E|U|A|P|R|S|F|                               |
   | Offset| Rsrvd |W|C|R|C|S|S|Y|I|            Window             |
   |       |       |R|E|G|K|H|T|N|N|                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Checksum            |         Urgent Pointer        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           [Options]                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               :
   :                             Data                              :
   :                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

          Note that one tick mark represents one bit position.
Figure 1: TCP Header Format

*/

#define LNET_TCP_HDR_LEN 20

#define LNET_TCP_FIN 0x01
#define LNET_TCP_SYN 0x02
#define LNET_TCP_RST 0x04
#define LNET_TCP_PSH 0x08
#define LNET_TCP_ACK 0x10
#define LNET_TCP_URG 0x20
#define LNET_TCP_ECE 0x40
#define LNET_TCP_WIN 0x80

struct tcphdr {
    uint16_t sport;             //  源端口号
    uint16_t dport;             //  目的端口号
    uint32_t seq;               //  报文开始序号
    uint32_t ack;               //  期望下一个开始的序号
    uint8_t rsvd : 4;           //  保留
    uint8_t hl : 4;             //  首部长度 4字节为单位
    uint8_t flags;              //  flags in little endian's cpu
                                //  low_address ----> high_address
                                //  FIN SYN RST PSH ACK URG ECE CWR
    uint16_t win;               //  允许的窗口
    uint16_t csum;              //  校验和
    uint16_t urp;               //  紧急数据
    uint8_t data[];             //  数据
}__attribute__((packed));

/*
                +--------+--------+--------+--------+
                |           Source Address          |
                +--------+--------+--------+--------+
                |         Destination Address       |
                +--------+--------+--------+--------+
                |  zero  |  PTCL  |    TCP Length   |
                +--------+--------+--------+--------+
Figure 2: IPv4 Pseudo-header

*/

/// @brief  伪头部，用来计算 tcp 校验和
struct pseudo_ip_in_tcp_csum {
    uint32_t saddr;             //  ip 源地址
    uint32_t daddr;             //  ip 目的地址
    uint8_t zero;               //  0
    uint8_t proto;              //  ip 中的协议字段
    uint16_t tlen;              //  tcp 头部 + 数据 的总长度 不包括这个 pseudo_header
}__attribute__((packed));


static inline void tcp_init(){
    assert(sizeof(struct tcphdr) == LNET_TCP_HDR_LEN);        //  20字节头部
}

void tcp_in(struct netdev *dev, struct eth_hdr *hdr);

void tcp_out(struct netdev *dev, struct eth_hdr *hdr);

int  tcp_checksum(struct iphdr *iphdr, struct tcphdr *tcphdr);

#endif