#include "netdev.h"
#include "ethernet.h"
#include "syshead.h"
#include "utils.h"
#include "tuntap_if.h"
#define BUF_LEN 100

void handle_frame(struct netdev * netdev, struct eth_hdr *hdr){

}

int main(){

  char buf[BUF_LEN];
  char *dev = calloc(10,1);
  struct netdev netdev;
  
  _utils_clear_array(buf);

  tun_init(dev);


  return 0;
}
