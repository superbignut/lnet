#### Reference:
---
[level-ip](https://github.com/saminiir/level-ip/blob/master/Documentation/getting-started.md)

[linux-docs](https://docs.kernel.org/networking/tuntap.html)

---
#### Pipeline:

        sudo make                       // change terminal
                                        
        arping -I lnet_tap_0 10.0.0.4   // sudo apt install iputils-arping 

        ping 10.0.0.4                   // 

