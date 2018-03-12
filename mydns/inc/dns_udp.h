//
// Created by Marcin Skwarek on 11.03.2018.
//
#ifndef AXFRSCANNER_DNS_UDP_H
#define AXFRSCANNER_DNS_UDP_H

#include "dns_packet.h"
#include "dns_response_parser.h"

typedef struct DNS_UDP_HEADER
{
    dns_header header;
} DNS_H_UDP;

dns_result dns_udp_req(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo, unsigned int to, char* host, char* buf,
                       int query_type, const char *server);

#endif //AXFRSCANNER_DNS_UDP_H
