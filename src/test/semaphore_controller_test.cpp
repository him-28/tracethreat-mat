#include "gtest/gtest.h"

#include "threadsyncocl/semaphore_controller.hpp"
#include "test/thread_controller_test.hpp"
#include "test/thread_exception_test.hpp"
#include "test/thread_barrier_controller_test.hpp"
#include "test/thread_sync_test.hpp"

#include "test/ac_tire_test.hpp"

#include "test/system_exception_test.hpp"

#include "test/file_handler_test.hpp"
#include "test/file_calculate_test.hpp"
#include "test/file_offset_handler_test.hpp" // Offset
#include "test/pe_file_controller_test.hpp"  // PE 

#include "test/wrapper_rules_tests.hpp"


// semaphore test suite 
using namespace controller;

TEST(semaphore_mutex_controller, mutex_controller)
{

	mutex_buffer<Mutex>  mutex_buff;
	// init 
	EXPECT_EQ(mutex_buff.init(), true);
	//
	EXPECT_EQ(mutex_buff.lock_request(), true);

	EXPECT_EQ(mutex_buff.unlock_request(), true);

	EXPECT_EQ(mutex_buff.destruction(), true);

}


GTEST_API_ int main(int argc, char ** argv)
{
 testing::InitGoogleTest(&argc, argv);
 testing::AddGlobalTestEnvironment( new ThreadSyncEnvironment(argc, argv));
 return RUN_ALL_TESTS();
}
