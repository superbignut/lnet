#include "syshead.h"



static int tun_fd;

static int tun_alloc(char *dev){
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0){
        
    }

}



    
void tun_init(char *dev){
    tun_fd = tun_alloc(dev);



}

void tun_read(char *buf, int len){



}

void tun_write(char *buf, int len){


}