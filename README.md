#### Reference:
---
[level-ip](https://github.com/saminiir/level-ip/blob/master/Documentation/getting-started.md)

[linux-docs](https://docs.kernel.org/networking/tuntap.html)

---
#### Pipeline:

        make        
        sudo mknod /dev/net/tap c 10 200
        sudo ./lnet
                                    // change terminal
        arping -I tap0 10.0.0.4     // sudo apt install iputils-arping 

