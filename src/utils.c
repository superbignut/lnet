#include "syshead.h"
#include "utils.h"


/// @brief 
/// @param fmt 打印错误 fmt
/// @param  
void _utils_print_error(const char *fmt, ...){
    va_list _ls;
    char buf[200];

    va_start(_ls, fmt);
    
    vsnprintf(buf, 200, fmt, _ls);
    
    va_end(_ls);
    perror(buf);
}

/// @brief system 执行命令 fmt
/// @param fmt 
/// @param  
/// @return 
int _utils_run_cmd(char *fmt, ...){
    va_list _ls;
    char buf[CMDBUFLEN];
    va_start(_ls, fmt);
    vsnprintf(buf, CMDBUFLEN, fmt, _ls);
    va_end(_ls);
    printf("%s\n", buf);
    return system(buf);
}

/// @brief unsigned char 打印 len 个 str 字符， 输出每行 8个字节
/// @param str 
/// @param len 
void _utils_print_hexdump(char *str, int len)
{
    printf("printing hexdump:\n");
    for (int i = 0; i < len; i ++) {
        if (i % 32 == 0){
            printf("\n");
        }
        printf("%02x ", (unsigned char)str[i]); // 如果不转为unsigned char 会被转为整形int32 
    }

    printf("\n");
}



/// @brief 由于 tcp udp有一个伪首部需要检验，因此提取出一个sum_every_16bits函数
/// @param addr 
/// @param count 
/// @return 
uint32_t sum_every_16bits(void *addr, int count){
    
    // printf("16 bits is %d\n", count);
    uint32_t sum = 0;

    uint16_t *ptr = addr;

    while(count > 1){
        sum += *ptr++;
        count -= 2;
    }

    if(count > 0){ // 奇数
        sum += *(uint8_t *) ptr;
    }

    return sum;
}


/// @brief 计算 addr 开始的 count 个字节的16位校验和: 求和后取反
/// @param addr 
/// @param count 
/// @return 
uint16_t _utils_check_sum(void *addr, int count, uint32_t start_sum){
    
    uint32_t sum = start_sum;

    sum += sum_every_16bits(addr, count);

    while(sum >> 16){
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return ~sum;
}


/// @brief 打印 u32 的 IP 地址
/// @param str 
/// @param addr 
void _utils_print_ip_addr(char* str, uint32_t addr){
    int addr_read[] = {0,0,0,0};
    for(int i = 0; i < 4; ++i){
        uint8_t tmp_addr = addr & 0xff;
        addr = addr >> 8;
        addr_read[i] = tmp_addr;
    }
    printf("%s: %d.%d.%d.%d\n", str, addr_read[0], addr_read[1], addr_read[2], addr_read[3]);
}


/// @brief 完成信号的注册
/// @param signo 
/// @param handler 
/// @return 
int _utils_signal_init(int signo, sighandler_t handler){
    struct sigaction sa;                    //  
    sigemptyset(&sa.sa_mask);               //  mask 被用来 block 当前 handler 执行时的其他信号
    sa.sa_flags = 0;
    sa.sa_flags |= SA_RESTART;              //  系统调用被信后中断时，会重启
    sa.sa_handler = handler;
    
    if(sigaction(signo, &sa, NULL) < 0){
        return -1;
    }
    return 0;
}


