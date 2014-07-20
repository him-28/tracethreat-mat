#include <iostream>
#include "gtest/gtest.h"

#include "environment_test.hpp"
#include "internet/logger/logging.hpp"
#include "internet/logger/stringprintf.hpp"

#include "internet/testinternet/test_updated_sign.hpp"



TEST(MappedFileController, TestIntial)
{
	 LOG(INFO)<<" Test Updated Signature Virus ";
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


