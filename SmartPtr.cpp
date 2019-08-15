#include <gtest/gtest.h>
#include <gmock/gmock.h>
//#include "SmartPtr.hpp"

using ::testing::_;
using ::testing::Return;

//#define DEBUG

#ifdef DEBUG
	#define sharedPtr std::shared_ptr
#else
	#include "SmartPtr.hpp"
#endif

namespace {

// The fixture for testing class Foo.
class sharedPtrTest : public ::testing::Test {
 protected:
  sharedPtrTest(){}
  ~sharedPtrTest(){}
  void testme(){}
  sharedPtr<int> sm_pointer;
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(sharedPtrTest, testme) {
  int i = 100;
  EXPECT_EQ(i, 100);

  int *int_p = new int(100);
	int *int_p_2 = new int(200);

	sharedPtr<int> ptr(int_p);
  EXPECT_EQ(ptr.use_count(), 1);
	sharedPtr<int> ptr_2(ptr);
	EXPECT_EQ(ptr.use_count(), 2);
	{
		sharedPtr<int> ptr_3(ptr);
		EXPECT_EQ(ptr.use_count(), 3);
	}
  EXPECT_EQ(ptr.use_count(), 2);
	//sharedPtr<int> ptr(int_p);
	//sharedPtr<int> ptr_2(ptr);
	//ptr = int_p;
	
	//printf("ptr_2.use_count = %d\n\n", ptr_2.use_count());
	EXPECT_EQ(ptr.use_count(), 2);

	sharedPtr<int> ptr_4(int_p_2);
	sharedPtr<int> ptr_5(ptr_4);
	ptr = ptr_4;
  EXPECT_EQ(ptr.use_count(), 3);
	EXPECT_EQ(ptr_4.use_count(), 3);
	//printf("9) ptr.get = %d\n\n", *ptr.get());
	//printf("10) ptr_4.get = %d\n\n", *ptr_4.get());
	EXPECT_EQ(ptr_2.use_count(), 1);

}



}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}