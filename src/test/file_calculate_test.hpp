#include "utils/file_calculate.hpp"

using namespace util;

class FileCalculateTest : public ::testing::Test
{
    protected:
        virtual void SetUp() {
            char *file_path = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils/logger";
            fc.set_filepath(file_path);
        }

        file_calculate<Extension> fc;
};


::testing::AssertionResult ispath(bool test)
{
    if(test)
        return ::testing::AssertionSuccess()<< "Is processes";
    else
        return ::testing::AssertionFailure()<< "Is cannot processes";
}


TEST_F(FileCalculateTest, RecurseFile)
{
    EXPECT_EQ(true, fc.processes());
		//ASSERT_PRED_FORMAT1(ispath, true);	
}

