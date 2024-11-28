/*
    https://docs.kernel.org/networking/tuntap.html

TUN/TAP provides packet reception and transmission for user space programs. It can be seen as a simple Point-to-Point
or Ethernet device, which, instead of receiving packets from physical media, receives them from user space program and
instead of sending packets via physical media writes them to the user space program.

In order to use the driver a program has to open /dev/net/tun and issue a corresponding ioctl() to register a network 
device with the kernel. A network device will appear as tunXX or tapXX, depending on the options chosen. When the 
program closes the file descriptor, the network device and all corresponding routes will disappear.

Depending on the type of device chosen the userspace program has to read/write IP packets (with tun) or ethernet frames (with tap). 
Which one is being used depends on the flags given with the ioctl().

*/



#include "syshead.h"
#include "utils.h"


static int tun_fd;


static int set_if_route(char *dev, char *cidr){
    return _utils_run_cmd("ip route add dev %s %s", dev, cidr);
}

static int set_if_up(char *dev){
    return _utils_run_cmd("ip link set dev %s up");
}

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
        exit(1);
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}



void tun_init(char *dev){
    tun_fd = tun_alloc(dev);
    if(set_if_up(dev) != 0){
        _utils_print_error("can't setup if");
    }
    if(set_if_route(dev, "10.0.0.0/24") != 0){
        _utils_print_error("can't set route");
    }
}

int tun_read(char *buf, int len){
    return read(tun_fd, buf, len);
}

int tun_write(char *buf, int len){
    return write(tun_fd, buf, len);
}

