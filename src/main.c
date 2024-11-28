#include "netdev.h"
#include "ethernet.h"
#include "syshead.h"
#include "utils.h"
#define BUF_LEN 100

void handle_frame(struct netdev * netdev, struct eth_hdr *hdr){

}

void help_func(char *str, ...){
  va_list _ls;
  va_start(_ls, str);

  
  printf("%s", str);
}

int main(){

  char buf[BUF_LEN];
  char *dev = calloc(10,1);
  struct netdev netdev;
  
  _utils_clear_array(buf);

  //tun_init()

  help_func("111", 123,123);
  
  return 0;
}
