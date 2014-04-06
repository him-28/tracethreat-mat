#include "threadsyncocl/thread_sync.hpp"
#include "threadsyncocl/buffer_sync.hpp"
#include "utils/config/options_system.hpp"
#include "utils/logger/clutil_logger.hpp"


#include "memory/file_shm_handler.hpp"


#include "memory/file_shm_handler.hpp"

#include "utils/file_offset_handler.hpp"

#include "threadsyncocl/buffer_sync.hpp"

#define FILE_SIZE_MULTIPLE_SHM 3

//namespace hnmav_util = hnmav_util;
using namespace memory;
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
            hnmav_util::options_system& op_system = hnmav_util::options_system::get_instance();
            op_system.generic_options();
            op_system.configure_options();
            op_system.process_options(argc, argv);

            if(!op_system.default_condition())
                std::cout<<" default " <<std::endl;

            //// init logger
            boost::shared_ptr<hnmav_util::clutil_logging<std::string, int> > *logger_ptr =
                    &hnmav_util::clutil_logging<std::string, int>::get_instance();
            //
            hnmav_util::clutil_logging<std::string, int> *logger = logger_ptr->get();
            std::string logger_main_file = op_system.get_logger_mainfile_path();
            std::string logger_settings = op_system.get_logger_settingsfile_path();
            //
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

            file_name_offset[0] = "/home/chatsiri/sda1/workspacemalware/malware_debug/vir_Win.Trojan.Zbot-15693/84612796/new_folder.exe";

            //initial file for file-shm
            file_name_offset[1] = "/home/chatsiri/sda1/workspacemalware/lab_malwareanalysis/3/clam_ISmsi_ext.exe";

						file_name_offset[2] = "/home/chatsiri/sda1/workspacemalware/malware_debug/infected_01.exe";

            for(int count_file = 0; count_file < 	FILE_SIZE_MULTIPLE_SHM; count_file++) {
                file_type_vec.push_back(file_name_offset[count_file]);
                s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));
                mapped_file_vec.push_back(s_mapped_fpe[count_file]);
                logger->write_info_test("file_path insert",
                        boost::lexical_cast<std::string>(file_name_offset[count_file]));
            }


        }

        //initial flies insert to file-shm
        const char *file_name_offset[FILE_SIZE_MULTIPLE_SHM];
        std::vector<const char*> file_type_vec;
        struct MAPPED_FILE_PE *s_mapped_fpe[FILE_SIZE_MULTIPLE_SHM];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;


        //logger
        boost::shared_ptr<hnmav_util::clutil_logging<std::string, int> > *logger_ptr;
        hnmav_util::clutil_logging<std::string, int>    *logger;


        typedef controller::BufferSync<
        struct controller::data_ocl_process<MAPPED_FILE_PE>,
                MAPPED_FILE_PE> buffer_sync;

        controller::thread_sync<buffer_sync, MAPPED_FILE_PE>  tsync;
};

TEST_F(ThreadSyncTest, sync_init)
{
    //logger->write_info_test(" Loop sync ");

    //intial file-shm
    utils::file_offset_handler<struct utils::common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;

    EXPECT_TRUE(fileoffset_h.mapped_file(file_type_vec, mapped_file_vec, fileoffset_h));

    uint64_t sum_file_size = 0;

    boost::shared_ptr<std::vector<MAPPED_FILE_PE * > > mappedf_vec_ptr =
            fileoffset_h.get_mappedf_vec_ptr();

    std::vector<MAPPED_FILE_PE *> *mapped_file_vec_ptr = mappedf_vec_ptr.get();

    //fileoffset_h.get_mapped_file();


    typename std::vector<MAPPED_FILE_PE *>::iterator iter_mapped_file;

    for(iter_mapped_file = mapped_file_vec_ptr->begin();
            iter_mapped_file != mapped_file_vec_ptr->end();
            ++iter_mapped_file) {

        MAPPED_FILE_PE *mf_pe = *iter_mapped_file;
        unsigned char *data = mf_pe->data;
        EXPECT_GT(mf_pe->size,0);

        //logger->write_info_test("Loop sync : mf_pe->size",
        //		boost::lexical_cast<std::string>(mf_pe->size));
        //logger->write_info_test("Loop sync : mf_pe->filename",
        //boost::lexical_cast<std::string>(mf_pe->file_name));

        sum_file_size += mf_pe->size;
        ASSERT_TRUE(*data != NULL);

    }

    file_shm_handler<MAPPED_FILE_PE>  f_shm_handler;
    f_shm_handler.initial_shm(sum_file_size);
    f_shm_handler.initial_file_shm(mapped_file_vec_ptr);




    //intial threads

    //typedef BufferSync<buffer_kernel>  buffer_sync;
    typedef controller::comm_thread_buffer<buffer_sync, MAPPED_FILE_PE>  comm_thread_buff;

    std::vector<boost::shared_ptr<comm_thread_buff> > thread_ptr_vec;
    //    thread_ptr_vec = t.sync_init();
    thread_ptr_vec = tsync.init_syncocl_workload(f_shm_handler.get_map_str_shm(),
            f_shm_handler.get_map_file_size());

    tsync.start_processes();



    /*
    ASSERT_NE(0, thread_ptr_vec.size());

    typename std::vector<boost::shared_ptr<comm_thread_buff> >::iterator iter;

    for(iter = thread_ptr_vec.begin(); iter != thread_ptr_vec.end(); ++iter) {
    boost::shared_ptr<comm_thread_buff> ct_buff = *iter;
    ct_buff->start();
    ct_buff->run();
    }

    tsync.sync_processes();
    */
    // delete file-shm after success compute.
    f_shm_handler.delete_file_shm();
    fileoffset_h.unmapped_file(*mapped_file_vec_ptr);
    //logger->write_info_test(" End Loop Sync ");

}

