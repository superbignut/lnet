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
