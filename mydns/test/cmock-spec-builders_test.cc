#include <cmock/cmock.h>

#include "mocks/testMock.h"

TEST(SpecBuildersTest, ExpectFunctionCallCompiles) {
  FooFunctionMock mock;
  EXPECT_FUNCTION_CALL(mock, (1, 2)).Times(0);
}

TEST(SpecBuildersTest, OnFunctionCallCompiles) {
  FooFunctionMock mock;
  ON_FUNCTION_CALL(mock, (1, 2));
}
