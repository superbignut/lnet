#include "syshead.h"
#include "utils.h"


static int tun_fd;

static int tun_alloc(char *dev){
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0){
        _utils_print_error("can't open TAP");
        exit(1);
    }
    
    _utils_clear_array(ifr);

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    if(*dev){
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if( (err = ioctl(fd, TUNSETIFF, (void*) &ifr)) < 0){
        _utils_print_error("can't ioctl TUN: %s\n", strerror(errno));
    }

    strcpy(dev, ifr.ifr_name);
    return fd;


}



    
void tun_init(char *dev){
    tun_fd = tun_alloc(dev);



}

void tun_read(char *buf, int len){



}

void tun_write(char *buf, int len){


}