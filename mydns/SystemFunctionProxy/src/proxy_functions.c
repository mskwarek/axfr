//
// Created by mskwarek on 21.06.18.
//

#include "../inc/proxy_functions.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int int_fcntl(int s, int cmd, long arg)
{
    int result = fcntl(s, cmd, arg);

    if (result < 0)
    {
        fprintf(stderr, "Error fcntl(...) (%s)\n", strerror(errno));
    }
    return result;
}

int sys_print_domain_info_to_file(char *output_buf, size_t output_buf_size, const char *na, int ttl, int type)
{
    return snprintf(output_buf, output_buf_size, "%s\t%d\t%d\t", na, ttl, type);
}
