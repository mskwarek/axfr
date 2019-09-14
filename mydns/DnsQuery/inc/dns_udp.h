//
// Created by Marcin Skwarek on 11.03.2018.
//
#ifndef AXFRSCANNER_DNS_UDP_H
#define AXFRSCANNER_DNS_UDP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dns_packet.h"
#include "dns_response_parser.h"

typedef struct DNS_UDP_HEADER
{
    dns_header header;
} DNS_H_UDP;

dns_result dns_udp_req(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo,
    unsigned int to, char *host, char *buf, int query_type, const char *server);
dns_result dns_req_with_spoofed_ip(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo,
    char *host, char *buf, int query_type, const char *server, const char *spoofed_ip);
dns_result dns_req_with_spoofed_ipv6(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo,
    char *host, char *buf, int query_type, const char *server, const char *spoofed_ip,
    const char *output_mac, int with_debug);

#ifdef __cplusplus
}
#endif

#endif // AXFRSCANNER_DNS_UDP_H
