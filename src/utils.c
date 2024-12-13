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

/// @brief 计算 addr 开始的 count 个字节的16位校验和: 求和后取反
/// @param addr 
/// @param count 
/// @return 
uint16_t _utils_check_sum(void *addr, int count){
    uint32_t sum = 0;

    uint16_t *ptr = addr;

    while(count > 1){
        sum += *ptr++;
        count -= 2;
    }

    if(count > 0){ // 奇数
        sum += *(uint8_t *)ptr;
    }

    while(sum >> 16){
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return ~sum;
    /*
        这里就相当于是一种计算方式，更多的细节需要严格的证明，但是简单的理解可以认为，
        如果原数据 是 0b0011， 校验和就是 0b1100, 求和之后再取反就一定是 0
        具体到底是先求和还是先取反，为什么进位要加回来，还有大小端问题，就先暂时不考虑吧
    */ 
}



void _utils_print_ip_addr(char* str, uint32_t addr){
    int addr_read[] = {0,0,0,0};
    for(int i = 0; i < 4; ++i){
        uint8_t tmp_addr = addr & 0xff;
        addr = addr >> 8;
        addr_read[i] = tmp_addr;
    }
    printf("%s: %d.%d.%d.%d\n", str, addr_read[0], addr_read[1], addr_read[2], addr_read[3]);
}