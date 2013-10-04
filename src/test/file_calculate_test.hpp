#include "utils/file_calculate.hpp"
#include "stdio.h"

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
		fc.file_size();
    int count = fc.get_count_file();
		ASSERT_NE(0, count);
		const char ** file_name = fc.get_file_cal();
		long  int  ** size      = fc.get_size_cal();
		for(int i = 0; i < count; i++)
		{
			long int * size_addr = size[i];
			std::stringstream ss;
			ss << std::hex << size_addr;
			unsigned int size_int = 0;
			ss >> size_int;
			std::cout<<" Test file : "<< file_name[i] << ", size : " << static_cast<int>(size_int) <<std::endl;			
		}
}

