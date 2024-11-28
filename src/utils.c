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