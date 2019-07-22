//
// Created by Marcin Skwarek on 08.03.2018.
//

#include "DnsResponseTest.hpp"
#include "dns.h"
#include "dns_tcp.h"
#include "dnsMock.h"
#include "SocketMock.h"
#include "SystemFunctionMock.h"

using namespace ::testing;

class DnsReceivedPacketReaderTest : public DnsResponseTest
{
};

TEST_F(DnsReceivedPacketReaderTest, testParsingResponse)
{
    {
        SocketMock socketMock{};

        EXPECT_CALL(socketMock, dns_tcp_req(_, _, _, _, _, _, _, _))
            .WillRepeatedly(Return(DNS_RESULT_OK));

        ngethostbyname(
            "example.domain.com", "10.0.0.1", "/var/log/path", QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP, RESPONSE_DUMP);
    }
}

TEST_F(DnsReceivedPacketReaderTest, testParsingValidResponse)
{
    {
        SocketMock socketMock{};
        DnsFunctionMock readAnswerMock{};
        SystemFunctionMock systemFunctionMock{};

        EXPECT_CALL(socketMock, dns_tcp_req(_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto size, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        EXPECT_CALL(systemFunctionMock, sys_print_buffer_to_file(_, _, _)).WillOnce(Return(0));
        EXPECT_CALL(readAnswerMock, readAnswers(_, _, _, _, _, _, _)).Times(1);

        EXPECT_EQ(DNS_RESULT_OK, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                     QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP, RESPONSE_DUMP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testDnsIdDoesNotMatch)
{
    {
        SocketMock socketMock{};

        EXPECT_CALL(socketMock, dns_tcp_req(_, _, _, _, _, _, _, _))
            .WillOnce(
                Invoke([this](auto, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    dnsByteBuffer[3] = 0;
                    dnsByteBuffer[2] = 0;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        EXPECT_EQ(DNS_RESULT_TID_ERR, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                      QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP, RESPONSE_DO_NOT_DUMP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testCannotOpenFile)
{
    {
        SocketMock socketMock{};
        SystemFunctionMock systemFunctionMock{};
        DnsFunctionMock readAnswerMock{};

        EXPECT_CALL(socketMock, dns_tcp_req(_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        EXPECT_CALL(systemFunctionMock, sys_print_buffer_to_file(_, _, _)).WillOnce(Return(1));
        EXPECT_CALL(readAnswerMock, readAnswers(_, _, _, _, _, _, _));
        EXPECT_EQ(DNS_RESULT_ERR, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                      QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP, RESPONSE_DUMP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testReadAnswers)
{
    {
        SocketMock socketMock{};
        SystemFunctionMock systemFunctionMock{};
        DnsFunctionMock readAnswersMock{};

        EXPECT_CALL(socketMock, dns_tcp_req(_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        EXPECT_CALL(systemFunctionMock, sys_print_buffer_to_file(_, _, _)).WillOnce(Return(0));
        EXPECT_CALL(readAnswersMock, readAnswers(_, _, _, _, _, _, _));
        EXPECT_EQ(DNS_RESULT_OK, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                     QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP, RESPONSE_DUMP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testNoMemory)
{
    {
        SocketMock socketMock{};
        SystemFunctionMock systemFunctionMock{};
        DnsFunctionMock readAnswersMock{};

        EXPECT_CALL(socketMock, dns_tcp_req(_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        EXPECT_CALL(systemFunctionMock, sys_print_buffer_to_file(_, _, _)).WillOnce(Return(0));
        EXPECT_CALL(readAnswersMock, readAnswers(_, _, _, _, _, _, _));
        EXPECT_EQ(DNS_RESULT_OK, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                     QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP, RESPONSE_DUMP));
    }
}
