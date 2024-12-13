/*
  变量里的数据全用大端， 只有比较的时候用htons\ntohs转成小端
*/


#include "netdev.h"
#include "ethernet.h"
#include "syshead.h"
#include "utils.h"
#include "tuntap_if.h"
#include "arp.h"
#include "ipv4.h"

#define BUF_LEN 100

void handle_frame(struct netdev *dev, struct eth_hdr *ethhdr)
{
    switch (ntohs(ethhdr->eth_type)) {
        case ETH_P_ARP:
            arp_incoming(dev, ethhdr);
            break;
        case ETH_P_IP:
            printf("Found IPv4\n");
            ipv4_incoming(dev, ethhdr);
            break;
        default:
            printf("Unrecognized ethertype %x\n", ethhdr->eth_type);
            break;
    }
}

int main(){

  char buf[BUF_LEN];
  
  char dev[15] = "lnet_tap_0";  //calloc(10,1);

  struct netdev net_device;
  
  _utils_clear_array(buf);  

  tun_tap_init(dev);

  network_device_init(&net_device, "10.0.0.4", "00:0c:29:6d:50:25");

  arp_init();

  while(1){

    if(tun_tap_read(buf, BUF_LEN) < 0){
      _utils_print_error("read from tun.");
    }

    _utils_print_hexdump(buf, BUF_LEN);

    struct eth_hdr *hdr = init_ethernet_header(buf); // 收到的

    handle_frame(&net_device, hdr);
  }

  return 0;
}
