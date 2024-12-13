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

/// @brief tun_tap 全局描述符
static int tun_tap_fd;


/// @brief 将端口置于 UP 状态， 使用ifconfig -a or ip link show 查看
/// @param dev interface name.
/// @return Error 1 
static int network_interface_set_up(char *dev){
    return _utils_run_cmd("ip link set dev %s up");
}


/// @brief 给端口添加一个路由
/// @param dev interface name.
/// @param cidr ip/mask
/// @return Error 1
static int network_interface_add_route(char *dev, char *cidr){
    return _utils_run_cmd("ip route add dev %s %s", dev, cidr);    // 这里如果不加 local 会增加一个 直连路由
}


/// @brief 
/// @param dev 
/// @param cidr 
/// @return 
static int network_interface_set_address(char *dev, char *cidr){
    return _utils_run_cmd("ip address add dev %s %s", dev, cidr);  // 默认就是 local
}


/// @brief 代码参考 linux 手册， 创建了一个 tap 端口
/// @param dev 
/// @return 
static int tun_tap_alloc(char *dev){
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0){
        _utils_print_error("can't open TAP");
        exit(1);
    }
    
    _utils_clear_array(ifr);

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;                    // 具体是 tun 还是tap 在这里体现

    if(*dev){                                               //如果名字第一位不是0， 则copy 过去
        // printf("yes\n");
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);               //tap0
    }

    if( (err = ioctl(fd, TUNSETIFF, (void*) &ifr)) < 0){    //
        _utils_print_error("can't ioctl TUN");
        exit(1);
    }
    strcpy(dev, ifr.ifr_name);                              // 如果最初传进去的名字是空的，会变成tap0
    return fd;
}


/// @brief 创建 tap 端口、打开端口、为端口添加路由
/// @param dev 
void tun_tap_init(char *dev){

    tun_tap_fd = tun_tap_alloc(dev);

    if(network_interface_set_up(dev) != 0){
        _utils_print_error("can't setup interface.");
    }

    if(network_interface_add_route(dev, "10.0.0.0/24") != 0){
        _utils_print_error("can't add route.");
    }

    if(network_interface_set_address(dev, "10.0.0.5/24") != 0){
        _utils_print_error("can't set ip addr.");
    }

}

/// @brief 从端口读
/// @param buf 
/// @param len 
/// @return 
int tun_tap_read(char *buf, int len){
    return read(tun_tap_fd, buf, len);
}

/// @brief 向端口写
/// @param buf 
/// @param len 
/// @return 
int tun_tap_write(char *buf, int len){
    return write(tun_tap_fd, buf, len);
}

