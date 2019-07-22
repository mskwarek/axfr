//
// Created by mskwarek on 19.06.18.
//

// #include <dnsMock.h>
// #include "dnsResponseFileWriterMock.h"
// #include <SystemFunctionMock.h>

#include "DnsResponseTest.hpp"
#include <cstring>
#include "dns.h"
#include "dns_tcp.h"

using namespace ::testing;

class DnsReadAnswersTest : public DnsResponseTest
{
};

// TEST_F(DnsReadAnswersTest, testReadingAnswers)
// {
//     //    unsigned char *dns = (struct DNS_TCP_HEADER*) &dnsByteBuffer;
//     struct RES_RECORD answers[2048] = {};
//     unsigned char *reader =
//         &dnsByteBuffer[sizeof(struct DNS_TCP_HEADER) +
//                        (strlen((const char *)&dnsByteBuffer[sizeof(struct DNS_TCP_HEADER)]) + 1) +
//                        sizeof(struct QUESTION)];

//     readAnswers(TRANSPORT_TYPE_TCP, reader, answers, dnsByteBuffer, NULL, 0, 0);
// }

// TEST_F(DnsReadAnswersTest, testReadingZonetransferMe)
// {
//     {
//         WriteMock writeMock{};
//         SystemFunctionMock systemFunctionMock{};

//         EXPECT_CALL(writeMock, write_raw_data(_, _, _)).WillRepeatedly(Return(0));
//         EXPECT_CALL(writeMock, write_endl(_, _)).WillRepeatedly(Return(0));
//         EXPECT_CALL(writeMock, write_rp_record(_, _, _, _)).WillRepeatedly(Return(0));
//         EXPECT_CALL(writeMock, write_string_to_file(_, _, _)).WillRepeatedly(Return(0));

//         struct RES_RECORD answers[2048] = {};
//         struct DNS_TCP_HEADER *dns = (struct DNS_TCP_HEADER *)&dnsByteBuffer;
//         unsigned char *reader =
//             &dnsByteBuffer[sizeof(struct DNS_TCP_HEADER) +
//                         (strlen((const char *)&dnsByteBuffer[sizeof(struct DNS_TCP_HEADER)]) + 1) +
//                         sizeof(struct QUESTION)];
//         EXPECT_CALL(systemFunctionMock, sys_print_domain_info_to_file(_, _, _, _, _)).WillRepeatedly(Return(0));
//         char output_buf[2048]= {0};
//         readAnswers(
//             TRANSPORT_TYPE_TCP, reader, answers, dnsByteBuffer, output_buf, sizeof(output_buf), get_answers_cnt(&dns->header));
//     }
// }
