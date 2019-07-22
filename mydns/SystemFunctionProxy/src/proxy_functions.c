//
// Created by mskwarek on 21.06.18.
//

#include "../inc/proxy_functions.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int int_fcntl(int s, int cmd, long arg)
{
    int result = -1;
    if(arg == 0)
        result = fcntl(s, cmd, NULL);
    else
        result = fcntl(s, cmd, arg);

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

int sys_print_buffer_to_file(const char* output_buf, size_t size, const char* filename)
{
    int tries = 0;
    FILE *f = NULL;
    do
    {
        f = fopen(filename, "w");
        sleep(3);
        tries++;
    } while (f == NULL && tries < 5);

    if (f != NULL)
    {
        fprintf(f, "%s", output_buf);
        fclose(f);
        return 0;
    }
    else
    {
        printf("Error opening file!\n");
        return 1;
    }
}
