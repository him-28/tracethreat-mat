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
        explicit ThreadSyncEnvironment(int argc,char **argv) {

            //thread_sync_file_path_arg = command_line_arg;

            util_opt::options_system& op_system = util_opt::options_system::get_instance();
            op_system.generic_options();
            op_system.configure_options();
            op_system.process_options(argc, argv);

            if(!op_system.default_condition())
                std::cout<<" default " <<std::endl;

            //std::string dir_path = op_system.get_scanfile_path();

            // init logger
            boost::shared_ptr<util_opt::clutil_logging<std::string, int> > *logger_ptr =
                    &util_opt::clutil_logging<std::string, int>::get_instance();

            util_opt::clutil_logging<std::string, int> *logger = logger_ptr->get();
            std::string logger_main_file = op_system.get_logger_mainfile_path();
            std::string logger_settings  = op_system.get_logger_settingsfile_path();

            logger->add_logger(logger_main_file);
            logger->add_settings(logger_settings);
            logger->init_backend();
            logger->formatter_normal();
            logger->init_frontend();

        }
};

class ThreadSyncTest : public ::testing::Test
{

    protected:
        virtual void SetUp() {
            util_opt::options_system& op_system = util_opt::options_system::get_instance();
            std::string dir_path = op_system.get_scanfile_path();
            ts.set_path(dir_path.c_str());
						
						//logger
						logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
						logger = logger_ptr->get();
						logger->write_info_test("Init logger for test");
        }

        //logger
        boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
        h_util::clutil_logging<std::string, int>    *logger;

        thread_sync<BufferSync<buffer_kernel> >  ts;
};

TEST_F(ThreadSyncTest, sync_init)
{
    logger->write_info_test(" Loop sync ");

    typedef BufferSync<buffer_kernel>  buffer_sync;
    typedef comm_thread_buffer<buffer_sync>  comm_thread_buff;

    std::vector<boost::shared_ptr<comm_thread_buff> > thread_ptr_vec;
    thread_ptr_vec = ts.sync_init();

    ASSERT_NE(0, thread_ptr_vec.size());

    typename std::vector<boost::shared_ptr<comm_thread_buff> >::iterator iter;

    for(iter = thread_ptr_vec.begin(); iter != thread_ptr_vec.end(); ++iter) {
        boost::shared_ptr<comm_thread_buff> ct_buff = *iter;
        ct_buff->start();
        ct_buff->run();
    }

    logger->write_info_test(" End Loop Sync ");
}

TEST_F(ThreadSyncTest, sync_thread_check)
{

    boost::tuple<buffer_kernel::size_int> value = ts.get_thread_info();
    EXPECT_EQ(1, boost::get<0>(value));
}
