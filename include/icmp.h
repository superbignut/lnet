#ifndef ICMP_H_
#define ICMP_H_
#include "syshead.h"
#include "netdev.h"
#include "ethernet.h"

/*
 +---------------+----------------+-------------------+-------------------+-----------------+
 |  new_ip_head  |  icmp_head_4B  |  icmp_special_4B  |  ori_ip_head_20B  |  ori_运输层_8B   | 
 +---------------+----------------+-------------------+-------------------+-----------------+
      ^
      |
    icmp   
*/


/// @brief icmp 前四个字节统一格式
struct icmp_v4 {
    uint8_t type;                   // 类型 
    uint8_t code;                   // code 用于进一步区分不同type
    uint16_t csum;                  // 校验和
    uint8_t data[];
} __attribute__((packed));

/*

Echo or Echo Reply Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Identifier          |        Sequence Number        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Data ...
   +-+-+-+-+-

*/
/// @brief 回送请求和回答报文
struct icmp_v4_echo {
    uint16_t id;                    // 
    uint16_t seq;                   //
    uint8_t data[];                 
} __attribute__((packed));


void icmpv4_incoming(struct netdev *dev, struct eth_hdr *ethhdr);

void icmpv4_reply(struct netdev *dev, struct eth_hdr *ethhdr);

#endif