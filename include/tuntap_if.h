#ifndef TUNTAP_IF_H_
#define TUNTAP_IF_H_

    void tun_init(char *dev);

    int tun_read(char *buf, int len);

    int tun_write(char *buf, int len);

#endif