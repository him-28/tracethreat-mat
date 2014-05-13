#include "gtest/gtest.h"
#include "environment_test.hpp"

//#include "threadsyncocl/semaphore_controller.hpp"
//#include "test/thread_controller_test.hpp"
//#include "test/thread_exception_test.hpp"
//#include "test/thread_barrier_controller_test.hpp"

//shm-read multiple fie
//#include "test/file_shm_handler_test.hpp"

//#include "test/file_offset_handler_test.hpp"

//#include "test/cli_scanner_wrapper_test.hpp"

//#include "test/cliprescan_pe_task_test.hpp"

#include "test/scan_pe_file_acp_test.hpp"

//#2 -stable
//#include "test/file_policy_test.hpp"         // PE fist file policy class

//#1
//#include "test/thread_sync_test.hpp"

/*
#include "test/ac_tire_test.hpp"
//
#include "test/actire_parallel_test.hpp"

#include "test/clutil_platform_test.hpp"

#include "test/system_exception_test.hpp"

#include "test/file_handler_test.hpp"


#include "test/file_calculate_test.hpp"


// Offset, Uncomment with test without
//  test/thread_sync_test.hpp  and test/file_shm_handler_test.hpp


//#include "test/pe_file_controller_test.hpp"  // PE


*/


//#include "test/wrapper_rules_tests.hpp"

#include "utils/config/options_system.hpp"
#include "utils/logger/clutil_logger.hpp"

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


GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment( new SystemTestingEnvironment(argc, argv));
    return RUN_ALL_TESTS();
}
