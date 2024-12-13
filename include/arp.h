/*
    RFC 826

    Packet format:
    --------------

    To communicate mappings from <protocol, address> pairs to 48.bit
    Ethernet addresses, a packet format that embodies the Address
    Resolution protocol is needed.  The format of the packet follows.

        Ethernet transmission layer (not necessarily accessible to
            the user):
            48.bit: Ethernet address of destination
            48.bit: Ethernet address of sender
            16.bit: Protocol type = ether_type$ADDRESS_RESOLUTION
// Use the same codes as listed in the section called "Ethernet Numbers of Interest" (all hardware types use this code set for the protocol type). --from ltl rfc1700
        Ethernet packet data:
            16.bit: (ar$hrd) Hardware address space (e.g., Ethernet,
                            Packet Radio Net.)
            16.bit: (ar$pro) Protocol address space.  For Ethernet
                            hardware, this is from the set of type
                            fields ether_typ$<protocol>.
            8.bit: (ar$hln) byte length of each hardware address
            8.bit: (ar$pln) byte length of each protocol address
            16.bit: (ar$op)  opcode (ares_op$REQUEST | ares_op$REPLY)
            nbytes: (ar$sha) Hardware address of sender of this
                            packet, n from the ar$hln field.
            mbytes: (ar$spa) Protocol address of sender of this
                            packet, m from the ar$pln field.
            nbytes: (ar$tha) Hardware address of target of this
                            packet (if known).
            mbytes: (ar$tpa) Protocol address of target.
*/


#ifndef ARP_H_
#define ARP_H_
#include "syshead.h"
#include "netdev.h"

#define LNET_ARP_ETHERNET   0x0001          // rfc826 用于确定硬件地址类型是不是Ethernet
#define LNET_ARP_IPV4       0x0800          // rfc1700 用于确定上层网络协议是不是ipv4

#define LNET_ARP_OP_REQUEST 0x0001          // rfc1700 对应 opcode字段
#define LNET_ARP_OP_REPLY   0x0002          // rfc1700 对应 opcode字段

#define LNET_ARP_CACHE_LEN   32             // cache 大小
#define LNET_ARP_CACHE_FREE        0        // cache 中使用的标记位
#define LNET_ARP_WAITING     1              // 暂未使用
#define LNET_ARP_CACHE_USED    2            // cache 中使用的标记位

#define LNET_ETH_ADD_LEN 6                  // 以太网地址字节数
#define LNET_IPV4_ADD_LEN 4                 // ip地址字节数

/// @brief arp 数据包 header 格式
struct arp_hdr
{
    uint16_t hwtype;            // 硬件地址类型， 支持多种硬件， ARP_ETHERNET
    uint16_t protype;           // 协议地址类型，对应以太网中的 eth_type ARP_IPV4
    unsigned char hwsize;       // 硬件地址字节长度， 支持多种硬件， 以太网是 6
    unsigned char prosize;      // 协议地址字节长度, ipv4 是 4
    uint16_t opcode;            // request / reply 请求还是回应
    unsigned char data[];       // 
} __attribute__((packed));

/// @brief arp 数据包的 ipv4 内容
struct arp_ipv4
{
    unsigned char smac[LNET_ETH_ADD_LEN];       // 发送的硬件地址
    uint32_t sip;                               // 源ip
    unsigned char dmac[LNET_ETH_ADD_LEN];       // 接受的硬件地址， 空
    uint32_t dip;                               // 目的ip
} __attribute__((packed));

/// @brief arp cache 缓存单元结构体, 但 hwtype 好像用不到欸， 如果只使用ip和eth， 那只需要更新ip：mac即可
struct arp_cache_entry
{
    uint16_t hwtype;                            // 硬件地址类型
    uint32_t sip;                               // 源 ip
    unsigned char smac[LNET_ETH_ADD_LEN];       // 源 mac
    unsigned int state;                         // 是否使用
};


void arp_init();

void arp_incoming(struct netdev *netdev, struct eth_hdr *hdr);

void arp_reply(struct netdev *netdev, struct eth_hdr *hdr, struct arp_hdr *arphdr);

#endif