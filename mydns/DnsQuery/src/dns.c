#include <string.h>
#include <sys/socket.h> //you know what this is for
#include <arpa/inet.h>
#include "dns.h"
#include "utils.h"
#include "dns_tcp.h"
#include "dns_udp.h"
#include "proxy_functions.h"

#include "dns_response_parser.h"
#include <stdio.h>
#include <netinet/in.h>

static int is_lack_of_memory(unsigned char *allocedMemory, int mallocRetry);

dns_result ngethostbyname(const char *que, const char *server, const char *dst_log_path,
    int query_type, int to, dns_transport_type transport_type, response_dump_to_file dump_to_file)
{
    unsigned int dns_id = 0;

    enum
    {
        BUFSIZE = 65536,
        HOSTSIZE = 128,
        FILENAME_SIZE = 512
    };
    unsigned char buf[BUFSIZE] = {0};
    int answers_cnt = 0;
    int dns_header_size = 0;

    // printf("%s %s\n", que, server);

    unsigned char *qname = NULL;
    struct QUESTION *qinfo = NULL;
    char host[HOSTSIZE] = {0};

    snprintf(host, HOSTSIZE, "%s", que);
    // printf("Resolving %s\n\n", host);

    if (TRANSPORT_TYPE_UDP == transport_type)
    {
        DNS_H_UDP *dns = NULL;
        dns = (struct DNS_UDP_HEADER *)&buf;
        feel_dns_header_req(&(dns->header));
        qname = &buf[sizeof(struct DNS_UDP_HEADER)];
        dns_id = dns->header.id;

        int res = dns_udp_req(dns, qname, qinfo, to, host, (char *)buf, query_type, server);
        if (DNS_RESULT_OK != res)
        {
            return res;
        }

        dns = (struct DNS_UDP_HEADER *)&buf;

        answers_cnt = ntohs(dns->header.ans_count);
        dns_header_size = sizeof(DNS_H_UDP);
    }
    else
    {
        DNS_H_TCP *dns = NULL;

        dns = (struct DNS_TCP_HEADER *)&buf;

        feel_dns_header_req(&(dns->header));
        qname = &buf[sizeof(struct DNS_TCP_HEADER)];
        dns_id = dns->header.id;

        int res = dns_tcp_req(dns, qname, qinfo, to, host, buf, query_type, server);
        if (DNS_RESULT_OK != res)
        {
            return res;
        }

        dns = (struct DNS_TCP_HEADER *)&buf;
        answers_cnt = ntohs(dns->header.ans_count);
        dns_header_size = sizeof(struct DNS_TCP_HEADER);
    }

    int i = 0;
    unsigned char *reader = NULL;

    char filename[FILENAME_SIZE] = {0};
    snprintf(filename, FILENAME_SIZE, "%s/%s_%s.axfr", dst_log_path, que, server);

    if (answers_cnt <= 0)
    {
        return DNS_RESULT_ERR;
    }

    if (TRANSPORT_TYPE_TCP == transport_type)
    {
        unsigned short dnsIdFromPacket = ((*(buf + 2) << 8) & 0xFF00) | (*(buf + 3) & 0xFF);

        if (htons(dns_id) != dnsIdFromPacket)
        {
            // printf("dnsId from resp does not match %d, %d, %d, %d, %d, %d", dns_id,
            // dnsIdFromPacket,
            //     htons(dns_id), ntohs(dns_id), htons(dnsIdFromPacket), ntohs(dnsIdFromPacket));
            return DNS_RESULT_TID_ERR;
        }
    }

    if (TRANSPORT_TYPE_UDP == transport_type)
    {
        unsigned short dnsIdFromPacket = ((*(buf) << 8) & 0xFF00) | (*(buf + 1) & 0xFF);

        if (htons(dns_id) != dnsIdFromPacket)
        {
            // printf("dnsId from resp does not match %d, %d, %d, %d, %d, %d", dns_id,
            // dnsIdFromPacket,
            //     htons(dns_id), ntohs(dns_id), htons(dnsIdFromPacket), ntohs(dnsIdFromPacket));
            return DNS_RESULT_TID_ERR;
        }
    }

    dns_result ret = DNS_RESULT_ERR;

    if (RESPONSE_DO_NOT_DUMP != dump_to_file)
    {
        char output_buf[BUFSIZE] = {0};

        struct RES_RECORD *answers = NULL;
        answers = (struct RES_RECORD *)calloc(answers_cnt, sizeof(struct RES_RECORD));
        if (answers == NULL)
        {
            return DNS_RESULT_NO_MEMORY;
        }

        reader = &buf[dns_header_size + (strlen((const char *)qname) + 1) + sizeof(struct QUESTION)];

        readAnswers(transport_type, reader, answers, buf, output_buf, sizeof(output_buf), answers_cnt);

        if (sys_print_buffer_to_file(output_buf, sizeof(output_buf), filename) == 0)
            ret = DNS_RESULT_OK;

        for (i = 0; i < answers_cnt; ++i)
        {
            if (answers[i].rdata != NULL)
                free(answers[i].rdata);
        }
        if (answers != NULL)
            free(answers);
    }
    return ret;
}

void readAnswers(dns_transport_type transport_type, unsigned char *reader,
    struct RES_RECORD *answers, unsigned char *buf, char *output_buf, size_t output_buf_size,
    int answers_cnt)
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

    size_t current_cursor_pos = 0;
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
            current_cursor_pos += sys_print_domain_info_to_file(output_buf + current_cursor_pos,
                output_buf_size - current_cursor_pos, (char *)na, ttl, type);
        }
        if ((DNS_RESULT_NO_MEMORY != ReadName(reader, name_size, type, buf + tcp_offset,
                                         output_buf + current_cursor_pos,
                                         output_buf_size - current_cursor_pos)) ||
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
