//
// Created by Marcin Skwarek on 20.02.2018.
//

#include <gmock/gmock.h>
#include "utils.h"
#include <string.h>

TEST(DnsUtilsTestSuite, ShouldDoNothingWhenNullsArePassed)
{
    unsigned char* passed = NULL;
    unsigned char* returned = NULL;

    ChangetoDnsNameFormat(passed, returned);
    EXPECT_EQ(passed, returned);
    EXPECT_TRUE(NULL == passed);
}

TEST(DnsUtilsTestSuite, ShouldChangeFormatWhenProperDataArePassed)
{
    unsigned char expected[25] = {'\x03', 'w', 'w', 'w', '\x06', 'g', 'o', 'o', 'g', 'l', 'e', '\x03', 'c', 'o', 'm'};
    unsigned char returned[25] = "";
    unsigned char passed[25] = { 'w', 'w', 'w', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'c', 'o', 'm'};

    ChangetoDnsNameFormat(returned, passed);
    EXPECT_TRUE(NULL != passed);
    EXPECT_TRUE(0 == memcmp((const char*)expected, (const char*)returned, sizeof(expected)));
}

TEST(DnsUtilsTestSuite, ShouldDoNothingWhenAddressWithDotAtBeginningIsPassed)
{
    unsigned char expected[25] = "";
    unsigned char returned[25] = "";
    unsigned char passed[25] = { '.', 'w', 'w', 'w', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'c', 'o', 'm'};

    ChangetoDnsNameFormat(returned, passed);
    EXPECT_TRUE(NULL != passed);
    EXPECT_TRUE(0 == memcmp((const char*)expected, (const char*)returned, sizeof(expected)));
}

TEST(DnsUtilsTestSuite, ShouldChangeFormatWhenDotAtLastPositionIsPassed)
{
    unsigned char expected[25] = {'\x03', 'w', 'w', 'w', '\x06', 'g', 'o', 'o', 'g', 'l', 'e', '\x03', 'c', 'o', 'm'};
    unsigned char returned[25] = "";
    unsigned char passed[25] = { 'w', 'w', 'w', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'c', 'o', 'm', '.'};

    ChangetoDnsNameFormat(returned, passed);
    EXPECT_TRUE(NULL != passed);
    EXPECT_TRUE(0 == memcmp((const char*)expected, (const char*)returned, sizeof(expected)));
}

TEST(DnsUtilsTestSuite, ShouldDoNothingWhenNullIsPassed)
{
    unsigned char passed[25] = {'\x03', 'w', 'w', 'w', '\x06', 'g', 'o', 'o', 'g', 'l', 'e', '\x03', 'c', 'o', 'm'};
    unsigned char returned[128] = "";

    int charsRead = readString(NULL, 128, passed, returned);
    EXPECT_TRUE(NULL != passed);
    EXPECT_EQ(0, charsRead);
}

TEST(DnsUtilsTestSuite, ShouldChangeFormatWhenProperDnsFormatIsPassed)
{
    unsigned char returned[128] = "";
    unsigned char expected[25] = { '\x06', 'n', 's', 'z', 't', 'm', '1', '\x04', 'd', 'i', 'g', 'i', '\x05', 'n',
                                   'i', 'n', 'j', 'a'};

    unsigned char dns_raw_soa_data[59] = { 0xc0, 0x0c, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x1c, 0x20, 0x00, 0x2f, 0x06, 0x6e,
    0x73, 0x7a, 0x74, 0x6d, 0x31, 0x04, 0x64, 0x69, 0x67, 0x69, 0x05, 0x6e, 0x69, 0x6e, 0x6a, 0x61, 0x00, 0x05, 0x72, 0x6f, 0x62,
    0x69, 0x6e, 0xc0, 0x34, 0x78, 0x39, 0x9e, 0x51, 0x00, 0x02, 0xa3, 0x00, 0x00, 0x00, 0x03, 0x84, 0x00, 0x12, 0x75, 0x00, 0x00,
    0x00, 0x0e, 0x10};

    readString(dns_raw_soa_data, sizeof(dns_raw_soa_data), dns_raw_soa_data, returned);
    EXPECT_TRUE(0 == memcmp((const char*)expected, (const char*)returned, sizeof(expected)));
}

TEST(DnsUtilsTestSuite, ShouldReadSOARecordWhenProperDnsFormatIsPassed)
{
    unsigned char returned[128] = "";
    unsigned char expected[25] = { 'n', 's', 'z', 't', 'm', '1', '.', 'd', 'i', 'g', 'i', '.', 'n',
                                   'i', 'n', 'j', 'a'};

    unsigned char dns_raw_soa_data[59] = { 0xc0, 0x0c, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x1c, 0x20, 0x00, 0x2f, 0x06,
                                           0x6e, 0x73, 0x7a, 0x74, 0x6d, 0x31, 0x04, 0x64, 0x69, 0x67, 0x69, 0x05, 0x6e,
                                           0x69, 0x6e, 0x6a, 0x61, 0x00, 0x05, 0x72, 0x6f, 0x62, 0x69, 0x6e, 0xc0, 0x34,
                                           0x78, 0x39, 0x9e, 0x51, 0x00, 0x02, 0xa3, 0x00, 0x00, 0x00, 0x03, 0x84, 0x00,
                                           0x12, 0x75, 0x00, 0x00, 0x00, 0x0e, 0x10};

    readSOA(dns_raw_soa_data, dns_raw_soa_data, returned);
    EXPECT_TRUE(0 == memcmp((const char*)expected, (const char*)returned, sizeof(expected)));
}
