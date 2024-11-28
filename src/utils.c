#include "syshead.h"
#include "utils.h"



void _utils_print_error(const char *fmt, ...){
    va_list _ls;
    char buf[200];

    va_start(_ls, fmt);
    
    vsnprintf(buf, 200, fmt, _ls);
    
    va_end(_ls);
    perror(buf);
}


int _utils_run_cmd(char *fmt, ...){
    va_list _ls;
    char buf[CMDBUFLEN];
    va_start(_ls, fmt);
    vsnprintf(buf, CMDBUFLEN, fmt, _ls);
    va_end(_ls);
    printf("%s\n", buf);
    return system(buf);
}

void _utils_print_hexdump(char *str, int len)
{
    printf("printing hexdump:\n");
    for (int i = 0; i < len; i ++) {
        if (i % 8 == 0){
            printf("\n");
        }
        printf("%02x ", (unsigned char)str[i]);
    }

    printf("\n");
}
