#include "utils/file_calculate.hpp"

using namespace util;

class FileCalculateTest : public ::testing::Test
{
        //        FileCalculateTest();
        //        ~FileCalculateTest();
    protected:
        virtual void SetUp() {
            char *file_path = "";
            fc.set_filepath(file_path);
        }

        file_calculate<Extension> fc;

        //        virtual void TearDown() {
};

TEST_F(FileCalculateTest, RecurseFile)
{
    EXPECT_EQ(true, fc.processes());
}

