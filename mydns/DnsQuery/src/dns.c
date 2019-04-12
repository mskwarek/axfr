#include <string.h>
#include <sys/socket.h> //you know what this is for
#include "../inc/dns.h"
#include "utils.h"
#include "../inc/dns_received_packet_reader.h"
#include "../inc/dns_tcp.h"
#include "../inc/dns_udp.h"

dns_result ngethostbyname(const char *que, const char *server, const char *dst_log_path,
    int query_type, int to, dns_transport_type transport_type)
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
    int q_count = 0;

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
        q_count = ntohs(dns->header.q_count);
        dns_header_size = sizeof(DNS_H_UDP);
    }
    else
    {
        DNS_H_TCP *dns = NULL;

        dns = (struct DNS_TCP_HEADER *)&buf;

        feel_dns_header_req(&(dns->header));
        qname = &buf[sizeof(struct DNS_TCP_HEADER)];
        dns_id = dns->header.id;

        int res = dns_tcp_req(dns, qname, qinfo, to, host, (char *)buf, query_type, server);
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
            // printf("dnsId from resp does not match %d, %d, %d, %d, %d, %d", dns_id, dnsIdFromPacket,
            //     htons(dns_id), ntohs(dns_id), htons(dnsIdFromPacket), ntohs(dnsIdFromPacket));
            return DNS_RESULT_TID_ERR;
        }
    }

    if (TRANSPORT_TYPE_UDP == transport_type)
    {
        unsigned short dnsIdFromPacket = ((*(buf) << 8) & 0xFF00) | (*(buf + 1) & 0xFF);

        if (htons(dns_id) != dnsIdFromPacket)
        {
            // printf("dnsId from resp does not match %d, %d, %d, %d, %d, %d", dns_id, dnsIdFromPacket,
            //     htons(dns_id), ntohs(dns_id), htons(dnsIdFromPacket), ntohs(dnsIdFromPacket));
            return DNS_RESULT_TID_ERR;
        }
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return DNS_RESULT_ERR;
    }

    struct RES_RECORD *answers = NULL;
    answers = (struct RES_RECORD *)calloc(answers_cnt, sizeof(struct RES_RECORD));
    if (answers == NULL)
    {
        fclose(f);
        return DNS_RESULT_NO_MEMORY;
    }

    reader = &buf[dns_header_size + (strlen((const char *)qname) + 1) + sizeof(struct QUESTION)];

    readAnswers(transport_type, reader, answers, buf, f, answers_cnt);

    fclose(f);
    for (i = 0; i < answers_cnt; ++i)
    {
        if (answers[i].rdata != NULL)
            free(answers[i].rdata);
    }
    if (answers != NULL)
        free(answers);
    return DNS_RESULT_OK;
}
