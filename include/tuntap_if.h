#ifndef TUNTAP_IF_H_
#define TUNTAP_IF_H_

    void tun_tap_init(char *dev);

    int tun_tap_read(char *buf, int len);

    int tun_tap_write(char *buf, int len);

#endif