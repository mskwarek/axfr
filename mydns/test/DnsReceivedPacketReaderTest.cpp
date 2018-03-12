//
// Created by Marcin Skwarek on 08.03.2018.
//

#include "DnsResponseTest.hpp"
#include "SocketMock.h"

using namespace ::testing;

class DnsReceivedPacketReaderTest : public DnsResponseTest
{

};

TEST_F(DnsReceivedPacketReaderTest, testParsingResponse)
{
    {
        SocketFunctionMock setoptionsMock;
        SendtoFunctionMock sentoMock;
        RecvfromFunctionMock recvfromMock;
        DnsTcpReceivedDataMock tcpMock;

//        EXPECT_FUNCTION_CALL(setoptionsMock, (_, _, _, _, _)).WillOnce(Return(0));
//        EXPECT_FUNCTION_CALL(sentoMock, (_, _, _, _, _, _)).WillOnce(Return(0));
//        EXPECT_FUNCTION_CALL(tcpMock, (_, _, _, _, _, _, _ ,_)).WillRepeatedly(
//            DoAll(
//                Return(0),
//                SetArgReferee<6>(this->dnsByteBuffer)));
    }

}

