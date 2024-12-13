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

/*
Type    Name                                    Reference
----    -------------------------               ---------
  0     Echo Reply                               [RFC792]
  1     Unassigned                                  [JBP]
  2     Unassigned                                  [JBP]
  3     Destination Unreachable                  [RFC792]
  4     Source Quench                            [RFC792]
  5     Redirect                                 [RFC792]
  6     Alternate Host Address                      [JBP]
  7     Unassigned                                  [JBP]
  8     Echo                                     [RFC792]
  9     Router Advertisement                    [RFC1256]
 10     Router Selection                        [RFC1256]
 11     Time Exceeded                            [RFC792]
 12     Parameter Problem                        [RFC792]
 13     Timestamp                                [RFC792]
 14     Timestamp Reply                          [RFC792]
 15     Information Request                      [RFC792]
 16     Information Reply                        [RFC792]
 17     Address Mask Request                     [RFC950]
 18     Address Mask Reply                       [RFC950]
 19     Reserved (for Security)                    [Solo]
 20-29  Reserved (for Robustness Experiment)        [ZSu]
 30     Traceroute                              [RFC1393]
 31     Datagram Conversion Error               [RFC1475]
 32     Mobile Host Redirect              [David Johnson]
 33     IPv6 Where-Are-You                 [Bill Simpson]
 34     IPv6 I-Am-Here                     [Bill Simpson]
 35     Mobile Registration Request        [Bill Simpson]
 36     Mobile Registration Reply          [Bill Simpson]
 37-255 Reserved                                    [JBP]
 */

#define LNET_ICMP_TYPE_ECHO 0x8
#define LNET_ICMP_TYPE_ECHO_REPLY 0x0



/// @brief icmp 前四个字节统一格式
struct icmp_v4 {
    uint8_t type;                   // 类型 
    uint8_t code;                   // code 用于进一步区分不同type
    uint16_t csum;                  // icmp校验和，包括type开始的所有内容
    uint8_t data[];
} __attribute__((packed));

//  icmp 校验和 : The checksum is the 16-bit ones's complement of the one's complement sum of the ICMP message starting with the ICMP Type.
//  For computing the checksum , the checksum field should be zero.
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