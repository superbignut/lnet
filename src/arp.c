#include "arp.h"

/// @brief arp 缓存： 保存局域网中 ip 和 mac 地址德映射表
static struct arp_cache_entry arp_cache[LNET_ARP_CACHE_LEN];

/// @brief 第一次见到的arp， 插入缓存
/// @param hdr 
/// @param data 
/// @return 
static int insert_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    struct arp_cache_entry *entry;
    for (int i = 0; i < LNET_ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == LNET_ARP_CACHE_FREE) {        // 找到空闲区域
            entry->state = LNET_ARP_CACHE_USED;           

            entry->hwtype = hdr->hwtype;                            // hwtype 
            entry->sip = data->sip;                                 // ip addr
            memcpy(entry->smac, data->smac, sizeof(entry->smac));   // mac addr

            return 0;
        }
    }

    return -1;
}

/// @brief 如果 已存在 cache 中， 更新 arp cache ， 返回1 ，else 0
/// @param hdr 
/// @param data 
/// @return 
static int update_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    struct arp_cache_entry *entry;

    for (int i = 0; i < LNET_ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == LNET_ARP_CACHE_FREE) continue;

        if (entry->hwtype == hdr->hwtype && entry->sip == data->sip) {
            memcpy(entry->smac, data->smac, sizeof(entry->smac));
            return 1;
        }
    }
    
    return 0;
}


/// @brief 清空 arp 缓存
static void arp_cache_clear(){
    memset(arp_cache, 0, LNET_ARP_CACHE_LEN * sizeof(struct arp_cache_entry));
}

/// @brief arp 初始化
void arp_init(){
    arp_cache_clear(); // 清空 arp_cache
}

/// @brief 处理收到的 arp 数据包，与本地ip进行匹配，判断是否 reply
/// @param netdev 
/// @param hdr 
void arp_incoming(struct netdev *netdev, struct eth_hdr *ethhdr)
{
    struct arp_hdr *tmp_arp_hdr;
    struct arp_ipv4 *tmp_arp_data;

    int merge = 0;

    tmp_arp_hdr = (struct arp_hdr *) ethhdr->payload;                  // arp header

    /*     tmp_arp_hdr->hwtype = ntohs(tmp_arp_hdr->hwtype);           // 网络字节序 转 小端
    tmp_arp_hdr->protype = ntohs(tmp_arp_hdr->protype);
    tmp_arp_hdr->opcode = ntohs(tmp_arp_hdr->opcode); */
              
    if (ntohs(tmp_arp_hdr->hwtype) != LNET_ARP_ETHERNET) {             // ?Do I have the hardware type in ar$hrd?  检查硬件地址类型是不是以太网
        printf("Unsupported HW type\n");
        return;
    }

    if (ntohs(tmp_arp_hdr->protype) != LNET_ARP_IPV4) {                // ?Do I speak the protocol in ar$pro? 检查 是不是 ipv4 协议
        printf("Unsupported protocol\n");
        return;
    }

    tmp_arp_data = (struct arp_ipv4 *) tmp_arp_hdr->data;       

    merge = update_arp_translation_table(tmp_arp_hdr, tmp_arp_data);

    if (netdev->addr != tmp_arp_data->dip) {                            // ?Am I the target protocol address?
        printf("ARP was not for us\n");
        return;
    }

    if (!merge && insert_arp_translation_table(tmp_arp_hdr, tmp_arp_data) != 0) {
       perror("ERR: No free space in ARP translation table\n"); 
    }

    switch (ntohs(tmp_arp_hdr->opcode)) {
    case LNET_ARP_OP_REQUEST:
        printf("Received ARP message target oneself. Start replying.\n");
        arp_reply(netdev, ethhdr, tmp_arp_hdr);
        break;
    default:
        printf("Opcode not supported\n");
        break;
    }
}

/// @brief 对需要回复的 arp 数据包做处理： ip mac 地址 修改，进而调用 transmit 发送
/// @param netdev 
/// @param hdr 
/// @param arphdr 
void arp_reply(struct netdev *netdev, struct eth_hdr *ethhdr, struct arp_hdr *arphdr) 
{
    struct arp_ipv4 *arpdata;
    int len;

    arpdata = (struct arp_ipv4 *) arphdr->data;

    memcpy(arpdata->dmac, arpdata->smac, LNET_ETH_ADDR_LEN);
    arpdata->dip = arpdata->sip;
    memcpy(arpdata->smac, netdev->hwaddr, LNET_ETH_ADDR_LEN);        // 源地址 用自己的hwaddr
    arpdata->sip = netdev->addr;                                    // 源ip 也用自己的

    // arphdr->opcode = LNET_ARP_OP_REPLY;

    arphdr->opcode = htons(LNET_ARP_OP_REPLY);
    // arphdr->hwtype =arphdr->hwtype;
    // arphdr->protype = arphdr->protype;

    len = sizeof(struct arp_hdr) + sizeof(struct arp_ipv4);
    netdev_transmit(netdev, ethhdr, ETH_P_ARP, len, arpdata->dmac);
}
