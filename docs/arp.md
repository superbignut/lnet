Pipeline:

1. linux 系统调用 创建 tap 接口
2. 打开 tap0 接口， 设定路由( 用不到，删掉配置也不影响 arping -I )
3. 读 tap0 , 不断检测 type 是不是 ARP， 是不是 ip 等匹配项
4. 写 tap0 , 进行回复

> 这里比较特别的地方是 ，接收到一个以太网数据包 struct eth_hdr *hdr； 最终发出去的还是这个数据包hdr，只不过是修改了不同的头部信息。

---
1. 另一个终端 使用 arping -I tap0 10.0.0.4 写接口
    这里的话，arping 这个命令写完应该还会不断监听 tap0， 进而得到回复

---

QA：
    
1. 以太网数据报 没有 最后的FCS
2. iptables 不清晰




