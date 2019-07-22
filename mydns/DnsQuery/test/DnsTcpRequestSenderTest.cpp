//
// Created by mskwarek on 20.06.18.
//

#include "dns_tcp.h"
#include "DnsResponseTest.hpp"
#include "SocketMock.h"
#include "dns.h"
#include "SystemFunctionMock.h"

using namespace ::testing;

class DnsTcpRequestSenderTest : public ::testing::Test
{
public:
    unsigned char buf[65535] = {0};
    unsigned char *qname = NULL;
    struct QUESTION *qinfo = NULL;
    DNS_H_TCP *dns = (struct DNS_TCP_HEADER *)&buf;
    char *query = "www.google.com";
    char host[128] = {0};

    virtual void setUp()
    {
        snprintf(host, 128, "%s", query);

        feel_dns_header_req(&(dns->header));
        qname = &buf[sizeof(struct DNS_TCP_HEADER)];
    }
};

TEST_F(DnsTcpRequestSenderTest, testSendingTcpRequest)
{
    setUp();
    dns_tcp_req(dns, qname, qinfo, 30, host, buf, QTYPE_AXFR, "192.168.0.1");
}

TEST_F(DnsTcpRequestSenderTest, testReturningErrorWhenNoSocketIsCreated)
{
    {
        SocketMock socketMock;
        EXPECT_CALL(socketMock, socket(_, _, _)).WillOnce(Return(-1));
        setUp();

        EXPECT_EQ(DNS_RESULT_ERR,
            dns_tcp_req(dns, qname, qinfo, 30, host, buf, QTYPE_AXFR, "192.168.0.1"));
    }
}

TEST_F(DnsTcpRequestSenderTest, testReturningErrorWhenSocketOptsAreNotSet)
{
    {
        SocketMock socketMock;

        EXPECT_CALL(socketMock, socket(_, _, _)).WillOnce(Return(0));
        EXPECT_CALL(socketMock, setsockopt(_, _, _, _, _)).WillOnce(Return(-1));

        setUp();

        EXPECT_EQ(DNS_RESULT_ERR,
            dns_tcp_req(dns, qname, qinfo, 30, host, buf, QTYPE_AXFR, "192.168.0.1"));
    }
}

TEST_F(DnsTcpRequestSenderTest, testReturningErrorWhenSocketCannotConnect)
{
    {
        SocketMock socketMock;
        SystemFunctionMock sysMock;

        EXPECT_CALL(socketMock, socket(_, _, _)).WillOnce(Return(0));
        EXPECT_CALL(socketMock, setsockopt(_, _, _, _, _)).WillOnce(Return(0));
        EXPECT_CALL(sysMock, int_fcntl(_, _, _)).WillOnce(Return(-1));
        EXPECT_CALL(socketMock, connect(_, _, _)).WillRepeatedly(Return(0));

        setUp();

        EXPECT_EQ(DNS_RESULT_ERR,
            dns_tcp_req(dns, qname, qinfo, 30, host, buf, QTYPE_AXFR, "192.168.0.1"));
    }
}

TEST_F(DnsTcpRequestSenderTest, testSocketSelectedToWrite)
{
    {
        SocketMock socketMock;
        SystemFunctionMock sysMock;

        EXPECT_CALL(socketMock, socket(_, _, _)).WillOnce(Return(0));
        EXPECT_CALL(socketMock, setsockopt(_, _, _, _, _)).WillOnce(Return(0));
        EXPECT_CALL(sysMock, int_fcntl(_, _, _)).WillOnce(Return(-1));
        EXPECT_CALL(socketMock, connect(_, _, _)).WillRepeatedly(Return(0));

        setUp();

        EXPECT_EQ(DNS_RESULT_ERR,
            dns_tcp_req(dns, qname, qinfo, 30, host, buf, QTYPE_AXFR, "192.168.0.1"));
    }
}
