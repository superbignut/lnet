#ifndef UTILS_H_
#define UTILS_H_

#define CMDBUFLEN 100

#define _utils_clear_array(x) memset(&(x), 0, sizeof(x))

void _utils_print_hexdump(char *str, int len);

void _utils_print_error(const char *str, ...);

int _utils_run_cmd(char *cmd, ...);

uint16_t _utils_check_sum(void *addr, int count);

#endif