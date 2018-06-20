//
// Created by mskwarek on 20.06.18.
//


#include <dnsMock.h>
#include "DnsResponseTest.hpp"
#include "SocketMock.h"
#include "../inc/dns.h"
#include "../inc/dns_tcp.h"
#include "FileMock.h"

using namespace ::testing;

class DnsTcpRequestSenderTest : public ::testing::Test
{

};

TEST_F(DnsTcpRequestSenderTest, testSendingTcpRequest)
{

    unsigned char buf[65535] = {0};
    DNS_H_TCP* dns = (struct DNS_TCP_HEADER *)&buf;
    struct QUESTION *qinfo = NULL;

    char *query = "www.google.com";

    char host[128] = {0};

    snprintf(host, 128, "%s", query);

    feel_dns_header_req(&(dns->header));
    unsigned char* qname = &buf[sizeof(struct DNS_TCP_HEADER)];

    dns_tcp_req(dns, qname, qinfo, 30,  host,
                           buf, QTYPE_AXFR, "192.168.0.1");
}