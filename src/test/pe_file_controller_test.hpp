//support File-SHM
#include "memory/file_shm_handler.hpp"
#include "filetypes/pe.hpp"
#include "utils/file_offset_handler.hpp"



#include "filetypes/pe_file_controller.hpp"
#include "utils/file_offset_handler.hpp"



#define FILE_SIZE 1

using namespace utils;
using namespace filetypes;
using namespace memory;

enum error_code { SUCCESS_COMPLETED_FILE, ERROR_CANNOT_OPEN, ERROR_PE_CORRUPT_FILE };

class PEFileControllerTest : public ::testing::Test
{
    protected:
        virtual void SetUp() {

            //load binary file pe.
            f_handler.file_full_path("/home/chatsiri/workspacecpp/pthread_sync_ocl/src/infected_file_test");
            //Get all file on path .
            std::vector<std::string> all_files = f_handler.get_full_path();
            //OpenCL kernel file.
            opencl_file_path    = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl/cl/tire_parallel.cl";
            //Signature file.
            file_sig            = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.hdb";

            //Load mapped_file object to vector.
            for(int count_file = 0; count_file < all_files.size(); count_file++) {
                file_type_vec.push_back(all_files[count_file].c_str());
                mapped_file_vec.push_back(new MAPPED_FILE_PE);
                s_mapped_fpe =  mapped_file_vec[count_file];
                s_mapped_fpe->msg_type = utils::internal_msg;
            }


        }

        //read file.
        std::vector<const char *> file_type_vec;
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;
        struct MAPPED_FILE_PE *s_mapped_fpe;
        const char *file_sig;
        std::string  opencl_file_path;

        //read file handler
        utils::file_handler<struct common_filetype> f_handler;


};


::testing::AssertionResult is_file_error(int  test_error)
{
    switch(test_error) {
    case SUCCESS_COMPLETED_FILE :
        return ::testing::AssertionSuccess()<< "Is get PE Header completed.";

    case ERROR_PE_CORRUPT_FILE :
        return ::testing::AssertionFailure()<< "Is cannot PE Header Corrupt file.";

    default:
        return ::testing::AssertionSuccess()<< "Is un match code.";
    }
}

TEST_F(PEFileControllerTest, pe_read_header)
{

    //File-SHM
    file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;

    EXPECT_TRUE(fileoffset_h.mapped_file(file_type_vec, mapped_file_vec, fileoffset_h, file_sig));

    size_t summary_file_size = 0;
    boost::shared_ptr<std::vector<MAPPED_FILE_PE *> > mappedf_vec_ptr =
            fileoffset_h.get_mappedf_vec_ptr();

    std::vector<MAPPED_FILE_PE *> *mapped_file_pe_vec = mappedf_vec_ptr.get();

    typename std::vector<MAPPED_FILE_PE *>::iterator iter_mapped_file;

    for(iter_mapped_file = mapped_file_pe_vec->begin();
            iter_mapped_file != mapped_file_pe_vec->end();
            ++iter_mapped_file) {
        MAPPED_FILE_PE *mf_pe = *iter_mapped_file;
        unsigned char *data = mf_pe->data;
        size_t size  = mf_pe->size;
        EXPECT_GT(size,0);
        summary_file_size += size;
        ASSERT_TRUE(*data != NULL);
    }

//    pe_file_controller<struct MAPPED_FILE_PE>  pe_file_col;
//		pe_file_col.get_pe_header(mapped_file_pe_vec);

};

