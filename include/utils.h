#ifndef UTILS_H_
#define UTILS_H_

#define CMDBUFLEN 100

#define _utils_clear_array(x) memset(&(x), 0, sizeof(x))

typedef void (*sighandler_t)(int);

void _utils_print_hexdump(char *str, int len);

void _utils_print_error(const char *str, ...);

int _utils_run_cmd(char *cmd, ...);

uint16_t _utils_check_sum(void *addr, int count, uint32_t start_sum);

uint32_t sum_every_16bits(void *addr, int count);

void _utils_print_ip_addr(char *str, uint32_t addr);

int _utils_signal_init(int signo, sighandler_t handler);
#endif