//
// Created by mskwarek on 19.06.18.
//

#include <dnsMock.h>
#include "DnsResponseTest.hpp"
#include "SocketMock.h"
#include "dnsResponseFileWriterMock.h"
#include "../inc/dns.h"
#include "../inc/dns_tcp.h"
#include "FileMock.h"
#include <sys/socket.h>
#include <SystemFunctionMock.h>

using namespace ::testing;

class DnsReadAnswersTest : public DnsResponseTest
{
};

TEST_F(DnsReadAnswersTest, testReadingAnswers)
{
    //    unsigned char *dns = (struct DNS_TCP_HEADER*) &dnsByteBuffer;
    struct RES_RECORD answers[2048] = {};
    unsigned char *reader =
        &dnsByteBuffer[sizeof(struct DNS_TCP_HEADER) +
                       (strlen((const char *)&dnsByteBuffer[sizeof(struct DNS_TCP_HEADER)]) + 1) +
                       sizeof(struct QUESTION)];

    readAnswers(TRANSPORT_TYPE_TCP, reader, answers, dnsByteBuffer, NULL, 0, 0);
}

TEST_F(DnsReadAnswersTest, testReadingZonetransferMe)
{
    WriteRawDataMock writeRawDataMock;
    WriteEndlMock writeEndlMock;
    WriteRpRecordMock writeRpRecordMock;
    WriteStringToFileMock writeStringToFileMock;
    PrintToFilelMock printToFilelMock;

    EXPECT_FUNCTION_CALL(writeRawDataMock, (_, _)).WillRepeatedly(Invoke([](auto, auto) {}));
    EXPECT_FUNCTION_CALL(writeEndlMock, (_)).WillRepeatedly(Invoke([](auto) {}));
    EXPECT_FUNCTION_CALL(writeRpRecordMock, (_, _, _)).WillRepeatedly(Invoke([](auto, auto, auto) {
    }));
    EXPECT_FUNCTION_CALL(writeStringToFileMock, (_, _)).WillRepeatedly(Invoke([](auto, auto) {}));

    struct RES_RECORD answers[2048] = {};
    struct DNS_TCP_HEADER *dns = (struct DNS_TCP_HEADER *)&dnsByteBuffer;
    unsigned char *reader =
        &dnsByteBuffer[sizeof(struct DNS_TCP_HEADER) +
                       (strlen((const char *)&dnsByteBuffer[sizeof(struct DNS_TCP_HEADER)]) + 1) +
                       sizeof(struct QUESTION)];
    EXPECT_FUNCTION_CALL(printToFilelMock, (_, _, _, _))
        .WillRepeatedly(Invoke([](auto, auto, auto, auto) {}));
    char output_buf[2048]= {0};
    readAnswers(
        TRANSPORT_TYPE_TCP, reader, answers, dnsByteBuffer, output_buf, sizeof(output_buf), get_answers_cnt(&dns->header));
}
