//
// Created by Marcin Skwarek on 08.03.2018.
//

#include "DnsResponseTest.hpp"
#include "SocketMock.h"
#include "../inc/dns.h"

using namespace ::testing;

class DnsReceivedPacketReaderTest : public DnsResponseTest
{

};

TEST_F(DnsReceivedPacketReaderTest, testParsingResponse)
{
    {
//        SocketFunctionMock setoptionsMock;
        SendtoFunctionMock sentoMock;
        RecvfromFunctionMock recvfromMock;
        DnsTcpReceivedDataMock tcpMock;

//        EXPECT_FUNCTION_CALL(setoptionsMock, (_, _, _, _, _)).WillOnce(Return(0));
//        EXPECT_FUNCTION_CALL(sentoMock, (_, _, _, _, _, _)).WillOnce(Return(0));
        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _ ,_)).Times(2).WillOnce(
//                Invoke([this](auto, auto, auto, auto, auto, unsigned char* buffer, auto, auto)
//                       {
//                           std::copy(std::begin(dnsByteBuffer), std::end(dnsByteBuffer), buffer);
//                           return DNS_RESULT_OK;
//                       })
        Return(DNS_RESULT_OK)
        );

        dns_result result = dns_tcp_req(NULL, NULL, NULL, 1, NULL, NULL, 0, NULL);

        ngethostbyname("example.domain.com", "10.0.0.1", "/var/log/path", QTYPE_AXFR, 30, TRANSPORT_TYPE_TCP);
    }

}

