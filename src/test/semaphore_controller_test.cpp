#include "gtest/gtest.h"
#include "threadsyncocl/semaphore_controller.hpp"

// semaphore test suite 
using namespace controller;
TEST(semaphore_mutex_controller, mutex_controller)
{

	mutex_buffer<Mutex>  mutex_buff;
	// init 
	EXPECT_EQ(mutex_buff.init(), true);
	//
	EXPECT_EQ(mutex_buff.lock_request(), true);
}

GTEST_API_ int main(int argc, char ** argv)
{
 testing::InitGoogleTest(&argc, argv);
 return RUN_ALL_TESTS();
}
