//
// Created by Marcin Skwarek on 08.03.2018.
//

#include <dnsMock.h>
#include <netinet/in.h>
#include "DnsResponseTest.hpp"
#include "SocketMock.h"
#include "../inc/dns.h"
#include "../inc/dns_tcp.h"
#include "../inc/dns_received_packet_reader.h"
#include "FileMock.h"
#include "SystemFunctionMock.h"
#include "dnsPacketReaderMock.h"

using namespace ::testing;

class DnsReceivedPacketReaderTest : public DnsResponseTest
{
};

TEST_F(DnsReceivedPacketReaderTest, testParsingResponse)
{
    {
        DnsTcpReceivedDataMock tcpMock;

        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _, _))
            .Times(2)
            .WillRepeatedly(Return(DNS_RESULT_OK));

        dns_result result = dns_tcp_req(NULL, NULL, NULL, 1, NULL, NULL, 0, NULL);

        ngethostbyname(
            "example.domain.com", "10.0.0.1", "/var/log/path", QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP);
    }
}

TEST_F(DnsReceivedPacketReaderTest, testParsingValidResponse)
{
    {
        //        SocketFunctionMock setoptionsMock;
        SendtoFunctionMock sentoMock;
        RecvfromFunctionMock recvfromMock;
        DnsTcpReceivedDataMock tcpMock;
        FileMock fileMock;
        DnsFunctionMock readAnswerMock;
        FileCloseMock fileCloseMock;

        //        EXPECT_FUNCTION_CALL(setoptionsMock, (_, _, _, _, _)).WillOnce(Return(0));
        //        EXPECT_FUNCTION_CALL(sentoMock, (_, _, _, _, _, _)).WillOnce(Return(0));
        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        FILE f;
        EXPECT_FUNCTION_CALL(fileMock, (_, _)).WillOnce(Return(&f));
        EXPECT_FUNCTION_CALL(readAnswerMock, (_, _, _, _, _, _));
        EXPECT_FUNCTION_CALL(fileCloseMock, (_)).WillOnce(Return(0));

        EXPECT_EQ(DNS_RESULT_OK, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                     QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testDnsIdDoesNotMatch)
{
    {
        DnsTcpReceivedDataMock tcpMock;

        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _, _))
            .WillOnce(
                Invoke([this](auto, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    dnsByteBuffer[3] = 0;
                    dnsByteBuffer[2] = 0;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        EXPECT_EQ(DNS_RESULT_ERR, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                      QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testCannotOpenFile)
{
    {
        DnsTcpReceivedDataMock tcpMock;
        FileMock fileMock;

        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        FILE *f = NULL;
        EXPECT_FUNCTION_CALL(fileMock, (_, _)).WillOnce(Return(f));
        EXPECT_EQ(DNS_RESULT_ERR, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                      QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testReadAnswers)
{
    {
        DnsTcpReceivedDataMock tcpMock;
        FileMock fileMock;
        PrintToFilelMock printToFileMock;
        DnsFunctionMock readAnswersMock;
        FileCloseMock fileCloseMock;

        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        FILE f;
        EXPECT_FUNCTION_CALL(fileMock, (_, _)).WillOnce(Return(&f));
        EXPECT_FUNCTION_CALL(printToFileMock, (_, _, _, _))
            .WillRepeatedly(Invoke([](auto, auto, auto, auto) {}));
        EXPECT_FUNCTION_CALL(readAnswersMock, (_, _, _, _, _, _))
            .WillRepeatedly(Invoke([](auto, auto, auto, auto, auto, auto) {}));
        EXPECT_FUNCTION_CALL(fileCloseMock, (_));
        EXPECT_EQ(DNS_RESULT_OK, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                     QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP));
    }
}

TEST_F(DnsReceivedPacketReaderTest, testNoMemory)
{
    {
        DnsTcpReceivedDataMock tcpMock;
        FileMock fileMock;
        PrintToFilelMock printToFileMock;
        IsLackOfMemoryMock isLackOfMemoryMock;
        DnsFunctionMock readAnswersMock;
        FileCloseMock fileCloseMock;

        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _, _))
            .WillOnce(Invoke(
                [this](auto dns_packet, auto, auto, auto, auto, unsigned char *buffer, auto, auto) {
                    auto dns = dns_packet->header.id;

                    dnsByteBuffer[3] = dns >> 8 & 0xFF;
                    dnsByteBuffer[2] = dns & 0xFF;
                    std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
                    return DNS_RESULT_OK;
                }));

        FILE f;
        EXPECT_FUNCTION_CALL(fileMock, (_, _)).WillOnce(Return(&f));
        EXPECT_FUNCTION_CALL(printToFileMock, (_, _, _, _))
            .WillRepeatedly(Invoke([](auto, auto, auto, auto) {}));
        EXPECT_FUNCTION_CALL(isLackOfMemoryMock, (_, _)).WillRepeatedly(Return(0));
        EXPECT_FUNCTION_CALL(readAnswersMock, (_, _, _, _, _, _))
            .WillRepeatedly(Invoke([](auto, auto, auto, auto, auto, auto) {}));
        EXPECT_FUNCTION_CALL(fileCloseMock, (_));
        EXPECT_EQ(DNS_RESULT_OK, ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path",
                                     QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP));
    }
}

// TEST_F(DnsReceivedPacketReaderTest, testReadAnswers)
//{
//    struct RES_RECORD answers[256] = {0};
//    struct DNS_TCP_HEADER *dns = (struct DNS_TCP_HEADER *)&dnsByteBuffer;
//    int ans_cnt = ntohs(dns->header.ans_count);
//    FILE f;
//    readAnswers(TRANSPORT_TYPE_TCP, dnsByteBuffer, answers, dnsByteBuffer, &f, 10);
//}