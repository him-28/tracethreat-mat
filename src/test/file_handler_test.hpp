#include "utils/file_handler.hpp"
using namespace util;

class FileHandlerTest : public ::testing::Test
{
    protected:

        virtual void SetUp() {
            file_name = "/home/chatsiri/Dropbox/reversing_engineer/malwarecookbook/3/3/clamav.yara";

        }

        const char *file_name;
        file_handler<common_filetype>  file_h;
				//file_handler<common_openfile_type> file_h;
        //	   g-test not support inheritance test
        //		ifile<common_filetype> * file_interface;// = new file_handler<common_filetype>();
        // 		file_interface = dynamic_cast<ifile<common_filetype>*>(&file_h);
        //		ifile<common_filetype>  * file_h =  dynamic_cast
        //			<ifile<common_filetype>* >(file_handler<common_filetype>());


};

TEST_F(FileHandlerTest, FileOpen)
{
    EXPECT_EQ(file_h.set_filepath(file_name), true);
    EXPECT_EQ(file_h.file_read(),true);
}

