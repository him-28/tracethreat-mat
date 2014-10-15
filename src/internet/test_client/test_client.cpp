#include <iostream>
#include "gtest/gtest.h"

#include "environment_test.hpp"
#include "internet/logger/logging.hpp"
#include "internet/logger/stringprintf.hpp"

//read signature.
//#include "internet/testinternet/test_updated_sign_test.hpp"

//internet connection
//#include "internet/testinternet/scan_server_connection_test.hpp"

//internet client connection
#include "internet/test_client/scan_client_connection_test.hpp"


TEST(InitialInternetTestSuite, Initial)
{
	 LOG(INFO)<<" Test Network Scanning service.";
}


void initial_logging(std::string filename_prefix)
{

    InitializeLogger(StringPrintf("%s.INFO", filename_prefix.c_str()),
            StringPrintf("%s.WARN", filename_prefix.c_str()),
            StringPrintf("%s.ERROR", filename_prefix.c_str()));
}

int _tmain(int argc, char  **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment( new SystemTestingEnvironment(argc, argv));
    return RUN_ALL_TESTS();
}


