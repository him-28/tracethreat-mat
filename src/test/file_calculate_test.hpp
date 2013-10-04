#include "utils/file_calculate.hpp"
#include "stdio.h"
#include "stdlib.h"

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

		int compare_function(const void * valuel, const void * valuer)
		{
			struct file_detail * valuel_ = (file_detail*)valuel;
			unsigned int  size_l = valuel_->size_cal;
			struct file_detail * valuer_ = (file_detail*)valuer;
			unsigned int  size_r = valuer_->size_cal;
			return size_l - size_r;
		}

TEST_F(FileCalculateTest, RecurseFile)
{
    EXPECT_EQ(true, fc.processes());
		fc.file_size();
    int count = fc.get_count_file();
		ASSERT_NE(0, count);
		struct file_detail * fd = fc.get_file_d();

		for(int i = 0; i < count; i++)
		{
			std::stringstream ss;
			ss << std::hex << fd[i].size_cal;
			unsigned int size_int = 0;
			ss >> size_int;
			std::cout<<" File str : " << fd[i].file_cal 
			<< ", size str : " << static_cast<int>(size_int) <<std::endl;
		}

		qsort(fd, count, sizeof(*fd), fd.compare_function);

		for(int i = 0; i < count; i++)
		{
			std::stringstream ss;
			ss << std::hex << fd[i].size_cal;
			unsigned int size_int = 0;
			ss >> size_int;
			std::cout<<" File str : " << fd[i].file_cal 
			<< ", size str : " << static_cast<int>(size_int) <<std::endl;
		}

}

