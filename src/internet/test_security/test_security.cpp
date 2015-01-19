#include <iostream>
#include "gtest/gtest.h"

#include "environment_test.hpp"
#include "internet/logger/logging.hpp"
#include "internet/logger/stringprintf.hpp"


TEST(InitialSecurityTestSuite, Initial)
{
	 LOG(INFO)<<" Test Security Module.";
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

//internet connection
//#include "internet/test_security/security_connection_test.hpp"

//AES controller
#include "internet/test_security/aes_controller_test.hpp"

//Field encrypted
//#include "internet/test_security/encryption_field_test.hpp"
