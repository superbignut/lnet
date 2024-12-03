#include "ethernet.h"
#include "syshead.h"


/// @brief 将 buf 转为 eth_hdr 格式， 并返回
/// @param buf 
struct ehc_hdr* init_ethernet_header(char * buf){

    struct eth_hdr * hdr = (struct eth_hdr *) buf;
    
    hdr->eth_type = htons(hdr->eth_type);   // 为什么要转换一下呢

    return hdr;
}