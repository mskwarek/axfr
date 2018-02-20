//
// Created by Marcin Skwarek on 20.02.2018.
//

#include <gmock/gmock.h>
#include "utils.h"
#include <string.h>

TEST(ChangetoDnsNameFormatUnitTest, ShouldDoNothingWhenNullsArePassed)
{
    unsigned char* passed = NULL;
    unsigned char* returned = NULL;

    ChangetoDnsNameFormat(passed, returned);
    EXPECT_EQ(passed, returned);
    EXPECT_TRUE(NULL == passed);
}

TEST(ChangetoDnsNameFormatUnitTest, ShouldChangeFormatWhenProperDataArePassed)
{
    unsigned char expected[25] = {'\x03', 'w', 'w', 'w', '\x06', 'g', 'o', 'o', 'g', 'l', 'e', '\x03', 'c', 'o', 'm'};
    unsigned char returned[25] = "";
    unsigned char passed[25] = { 'w', 'w', 'w', '.', 'g', 'o', 'o', 'g', 'l', 'e', '.', 'c', 'o', 'm'};

    ChangetoDnsNameFormat(returned, passed);
    EXPECT_TRUE(NULL != passed);
    EXPECT_TRUE(0 == memcmp((const char*)expected, (const char*)returned, sizeof(expected)));
}


