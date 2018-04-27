#include <gtest/gtest.h>
#include "DnsResponseTest.hpp"


TEST(Dummy, DummyAssertForEnvCheck)
{
    ASSERT_EQ(1, 1);
}

class DnsMainScanTest : public DnsResponseTest
{

};

TEST_F(DnsMainScanTest, getProperDnsPacketFromSocketTest)
{

}


