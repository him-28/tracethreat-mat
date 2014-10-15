#include "utils/file_handler.hpp"
using namespace utils;

class FileHandlerTest : public ::testing::Test
{
    protected:

        virtual void SetUp() {
            file_name = "/home/chatsiri/Dropbox/reversing_engineer/malwarecookbook/3/3/clamav.yara";

        }

        const char *file_name;
        file_handler<common_filetype>  file_h;

};

TEST_F(FileHandlerTest, FileOpen)
{
    EXPECT_EQ(file_h.set_filepath(file_name), true);
    EXPECT_EQ(file_h.file_read(),true);
}

