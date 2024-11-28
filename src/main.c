#include "netdev.h"
#include "ethernet.h"
#include "syshead.h"
#include "utils.h"
#include "tuntap_if.h"
#include "arp.h"

#define BUF_LEN 100

void handle_frame(struct netdev *netdev, struct eth_hdr *hdr)
{
    switch (hdr->ether_type) {
        case ETH_P_ARP:
            arp_incoming(netdev, hdr);
            break;
        case ETH_P_IP:
            printf("Found IPv4\n");
            break;
        default:
            printf("Unrecognized ethertype %x\n", hdr->ether_type);
            break;
    }
}

int main(){

  char buf[BUF_LEN];
  char *dev = calloc(10,1);
  struct netdev netdev;
  
  _utils_clear_array(buf);

  tun_init(dev);

  netdev_init(&netdev, "10.0.0.4", "00:0c:29:6d:50:25");

  arp_init();


  while(1){
    if(tun_read(buf, BUF_LEN) < 0){
      _utils_print_error("read from tun.");
    }
    _utils_print_hexdump(buf, BUF_LEN);

    struct eth_hdr *hdr = init_eth_hdr(buf);

    handle_frame(&netdev, hdr);
  }

  return 0;
}
