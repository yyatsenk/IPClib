#include "gtest/gtest.h"
#include "SmartPtr.hpp"


namespace {

// The fixture for testing class Foo.
class sharedPtrTest : public ::testing::Test {
 protected:
  sharedPtrTest(){}
  ~sharedPtrTest(){}
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(sharedPtrTest, sharedPtrTest) {
  int i = 100;
  EXPECT_EQ(i, 100);
}



}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}