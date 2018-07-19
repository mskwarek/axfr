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
    int result =  fcntl(s, cmd, arg);

    if (result < 0 )
    {
        fprintf(stderr, "Error fcntl(...) (%s)\n", strerror(errno));
    }
    return result;
}

void sys_print_domain_info_to_file(FILE *f, const char* na, int ttl, int type)
{
    fprintf(f, "%s\t%d\t%d\t", na, ttl, type);
}