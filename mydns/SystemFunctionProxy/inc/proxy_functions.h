//
// Created by mskwarek on 21.06.18.
//

#ifndef AXFRSCANNER_PROXY_FUNCTIONS_H
#define AXFRSCANNER_PROXY_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

int int_fcntl(int s, int cmd, long arg);
int sys_print_domain_info_to_file(char *output_buf, size_t output_buf_size, const char *na, int ttl, int type);

#ifdef __cplusplus
}
#endif

#endif // AXFRSCANNER_PROXY_FUNCTIONS_H
