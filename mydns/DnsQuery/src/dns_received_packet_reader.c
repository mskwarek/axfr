//
// Created by Marcin Skwarek on 03.03.2018.
//

#include "../inc/dns_received_packet_reader.h"
#include "../inc/dns_response_parser.h"
#include "../../SystemFunctionProxy/inc/proxy_functions.h"
#include <stdio.h>
#include <netinet/in.h>
#include "utils.h"

void readAnswers(dns_transport_type transport_type, unsigned char *reader,
    struct RES_RECORD *answers, unsigned char *buf, FILE *f, int answers_cnt)
{
    // Start reading answers

    int i = 0;
    int noMemory = 0;
    int mallocRetry = 0;

    unsigned char na[1024] = {0};
    unsigned int tcp_offset = 0;
    if (TRANSPORT_TYPE_TCP == transport_type)
    {
        tcp_offset = 2;
    }

    unsigned int max_len = sizeof(struct RES_RECORD) * (answers_cnt);
    unsigned int name_offset = readSOA(reader, buf + tcp_offset, na, max_len) + 1;

    //    	unsigned short class = ((*(reader+2 + name_offset) << 8) &0xFF00) | (*(reader+3 +
    //    name_offset) & 0xFF);
    unsigned short type =
        ((*(reader + name_offset) << 8) & 0xFF00) | (*(reader + 1 + name_offset) & 0xFF);
    unsigned int ttl = parse_to_uint(reader + 4 + name_offset);
    unsigned short name_size =
        ((*(reader + 8 + name_offset) << 8) & 0xFF00) | (*(reader + 9 + name_offset) & 0xFF);

    for (i = 0; i < answers_cnt;)
    {
        answers[i].resource = (struct R_DATA *)(reader);

        reader += 10 + name_offset;

        answers[i].rdata = (unsigned char *)malloc(name_size * sizeof(unsigned short));
        if (1 == is_lack_of_memory(answers[i].rdata, mallocRetry))
        {
            mallocRetry++;
        }
        else
        {
            sys_print_domain_info_to_file(f, na, ttl, type);
        }
        if ((DNS_RESULT_NO_MEMORY != ReadName(reader, name_size, type, buf + tcp_offset, f)) ||
            noMemory > 2)
        {
            reader += name_size;
            noMemory = 0;
            mallocRetry = 0;
            ++i;
        }
        else
        {
            noMemory++;
        }
    }
}

int is_lack_of_memory(unsigned char *allocedMemory, int mallocRetry)
{
    return (allocedMemory == NULL && mallocRetry < 4);
}

int get_answers_cnt(dns_header *dns)
{
    return ntohs(dns->ans_count);
}
