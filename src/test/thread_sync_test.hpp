#include "threadsyncocl/thread_sync.hpp"
#include "threadsyncocl/buffer_sync.hpp"


class ThreadSyncTest : public ::testing::Test
{

		protected:
				virtual void SetUp(){
				  char * file_path = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils/logger";
					ts.set_path(file_path);		

				}

  			thread_sync<BufferSync<buffer_kernel> >  ts;
};

TEST_F(ThreadSyncTest, sync_init)
{
  ASSERT_EQ(true, ts.sync_init());
}

TEST_F(ThreadSyncTest, sync_thread_check)
{

	boost::tuple<buffer_kernel::size_int> value = ts.get_thread_info();
	EXPECT_EQ(1, boost::get<0>(value));
}
