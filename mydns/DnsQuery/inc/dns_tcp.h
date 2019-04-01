//
// Created by Marcin Skwarek on 11.03.2018.
//

#ifndef AXFRSCANNER_DNS_TCP_H
#define AXFRSCANNER_DNS_TCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dns_packet.h"
#include "dns_response_parser.h"

typedef struct DNS_TCP_HEADER
{
    unsigned short len;
    dns_header header;
} DNS_H_TCP;

dns_result dns_tcp_req(DNS_H_TCP *dns, unsigned char *qname, struct QUESTION *qinfo,
    unsigned int to, char *host, unsigned char *buf, int query_type, const char *server);

#ifdef __cplusplus
}
#endif
#endif // AXFRSCANNER_DNS_TCP_H
