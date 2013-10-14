#include "threadsyncocl/thread_sync.hpp"
#include "threadsyncocl/buffer_sync.hpp"
#include "utils/config/options_system.hpp"

namespace util_opt = hnmav_util;

/*
namespace
{

    std::string thread_sync_file_path_arg;
}
*/

class ThreadSyncEnvironment : public testing::Environment
{
    public:
        explicit ThreadSyncEnvironment(int argc,char ** argv) {
					
            //thread_sync_file_path_arg = command_line_arg;
					
            util_opt::options_system& op_system = util_opt::options_system::get_instance();
            op_system.generic_options();
            op_system.configure_options();
            op_system.process_options(argc, argv);

            if(!op_system.default_condition())
                std::cout<<" default " <<std::endl;

            //std::string dir_path = op_system.get_scan_path();
				
        }
};

class ThreadSyncTest : public ::testing::Test
{

    protected:
        virtual void SetUp() {
            char *file_path = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/utils/logger";
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
