#include "syshead.h"
#include "utils.h"



void print_error(char *str, ...){
    va_list _ls;
    char buf[200];

    va_start(_ls, str);
    
    vsnprintf(buf, 200, str, _ls);
    
    va_end(_ls);
    perror(_ls);

    



}