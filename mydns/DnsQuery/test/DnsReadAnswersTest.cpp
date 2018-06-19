//
// Created by mskwarek on 19.06.18.
//


#include <dnsMock.h>
#include "DnsResponseTest.hpp"
#include "SocketMock.h"
#include "../inc/dns.h"
#include "../inc/dns_tcp.h"
#include "FileMock.h"

using namespace ::testing;

class DnsReadAnswersTest : public DnsResponseTest
{

};

TEST_F(DnsReadAnswersTest, testReadingAnswers)
{
//    unsigned char *dns = (struct DNS_TCP_HEADER*) &dnsByteBuffer;
    struct RES_RECORD answers[2048] = {};
    unsigned char *reader = &dnsByteBuffer[sizeof(struct DNS_TCP_HEADER) +
            (strlen((const char*)&dnsByteBuffer[sizeof(struct DNS_TCP_HEADER)])+1) + sizeof(struct QUESTION)];

    readAnswers(TRANSPORT_TYPE_TCP, reader, answers, dnsByteBuffer, NULL, 0);
}