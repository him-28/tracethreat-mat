#include <iostream>
#include "gtest/gtest.h"

#include "environment_test.hpp"
#include "internet/logger/logging.hpp"
#include "internet/logger/stringprintf.hpp"


TEST(InitialInternetTestSuite, Initial)
{
	 LOG(INFO)<<" Test RPC-Message service.";
}


void initial_logging(std::string filename_prefix)
{

    InitializeLogger(StringPrintf("%s.INFO", filename_prefix.c_str()),
            StringPrintf("%s.WARN", filename_prefix.c_str()),
            StringPrintf("%s.ERROR", filename_prefix.c_str()));
}

GTEST_API_ int main(int argc, char  **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment( new SystemTestingEnvironment(argc, argv));
    return RUN_ALL_TESTS();
}

//Send message
#include "internet/test_rpc/sendmsg_rpc_client_test.hpp"
