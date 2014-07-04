#include "threadconcurrency/tbbpostscan_pe_controller.hpp"
#include "tbb/concurrent_vector.h"
#include "utils/base/system_code.hpp"

//support File-SHM
#include "memory/file_shm_handler.hpp"
#include "filetypes/pe.hpp"
#include "utils/file_offset_handler.hpp"

//File Number
#define FILE_ON 3


using tbbscan::actire_pe_engine;

using namespace memory;
using namespace utils;


class TBBPostScanPEController : public  ::testing::Test
{

    protected:

        virtual void SetUp() {

            std::string sig1 = "09cd21b80";
            std::string sig2 = "09cd21b44ce1aea";
            /*
            char *binary_hex="a500e6b7a4b523235323234ae12b1e8bb";

            memcpy(&binary_hex_input[*binary_hex_input.grow_by(strlen(binary_hex))],
            binary_hex,
            strlen(binary_hex)+1);
            */
            meta_sig_vec.push_back(new struct utils::meta_sig);
            msig = meta_sig_vec[0];
            msig->sig  = "09cd21b80abe3"; //sig_node to sig
            msig->virname   = "test_trojan_a";
            msig->sig_type  = utils::pe_file;
            msig->sig_size  = strlen(msig->sig);
            sig_key_vec.push_back(std::string(msig->sig));

            meta_sig_vec.push_back(new struct utils::meta_sig);
            msig = meta_sig_vec[1];
            msig->sig  = "09cd21b44ce1aea";
            msig->virname   = "test_trojan_b";
            msig->sig_type  = utils::pe_file;
            msig->sig_size  = strlen(msig->sig);
            sig_key_vec.push_back(std::string(msig->sig));

            meta_sig_vec.push_back(new struct utils::meta_sig);
            msig = meta_sig_vec[2];
            msig->sig  = "235323234ae12b1e8";
            msig->virname   = "test_trojan_c";
            msig->sig_type  = utils::pe_file;
            msig->sig_size  = strlen(msig->sig);
            sig_key_vec.push_back(std::string(msig->sig));

            meta_sig_vec.push_back(new struct utils::meta_sig);
            msig = meta_sig_vec[3];
            msig->sig  = "2353ab1e119d32c67a23ab3d";
            msig->virname   = "test_trojan_d";
            msig->sig_type  = utils::pe_file;
            msig->sig_size  = strlen(msig->sig);
            sig_key_vec.push_back(std::string(msig->sig));

            meta_sig_vec.push_back(new struct utils::meta_sig);
            msig = meta_sig_vec[4];
            msig->sig  = "4d5a50000200000004000f00ffff0000b80000000000000040001a000000";
            msig->virname   = "trojan-zbot-1563";
            msig->sig_type  = utils::pe_file;
            msig->sig_size  = strlen(msig->sig);
            sig_key_vec.push_back(std::string(msig->sig));

            sig_type = utils::pe_file;


            file_name_offset[0] = "/home/chatsiri/sda1/workspacemalware/malware_debug/vir_Win.Trojan.Zbot-15693/84612796/new_folder.exe";
            file_name_offset[1] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_01.exe";

            file_name_offset[2] = "/home/chatsiri/sda1/workspacemalware/lab_malwareanalysis/3/clam_ISmsi_ext.exe";

            file_sig = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.hdb";

            for(int count_file = 0; count_file < 	FILE_ON; count_file++) {
                file_type_vec.push_back(file_name_offset[count_file].c_str());
                mapped_file_vec.push_back(&s_mapped_fpe[count_file]);
            }


        }


        //Support Task Scanning.
        std::vector<std::string> sig_key_vec;

        //binary input from file (Test only)
        //tbb::concurrent_vector<char> binary_hex_input;

        std::vector<std::string> keywords;

        //Node of tree.
        std::vector<struct utils::meta_sig *>  meta_sig_vec;

        //Output function.
        tbb::concurrent_unordered_map<std::size_t,
            std::set<struct utils::meta_sig *> > output_fn;

        struct utils::meta_sig *msig;

        memory::signature_shm_pe_controller<struct memory::meta_sig, struct memory::meta_sig_mem>
                    sig_shm_pe;
        uint8_t sig_type;

        //support File-SHM
        std::vector<const char *> file_type_vec;
        struct MAPPED_FILE_PE s_mapped_fpe[FILE_ON];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;
        std::string  file_name_offset[FILE_ON];
        const char *file_sig;


};



TEST_F(TBBPostScanPEController, InitialScanEngine)
{
    //Step initial scanning with multithread.
    //[x] Initial vector<struct MAPPED_FILE_PE*> contains all file for scanning.
    //[x] Initial Signature SHM.
    //[x] Initial File Input (Multiple file in vector).
    //[x] Initial FILE-SHM.
    //[x] Initial actire_engine (Signature Engine).
    //[x] Initial actire_engine_factory for get search engine per type such PE,EXE, ELF and Processes.
    //[] Initial thread per task.
    //[] Multiple tasks per threads.

    //Signature-SHM
    std::string shm_name  = "shm-pe";
    sig_shm_pe.initial_shm_sigtype(&meta_sig_vec, shm_name);

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


    memory::file_shm_handler<struct MAPPED_FILE_PE>  f_shm_handler;
    f_shm_handler.initial_shm(summary_file_size);
    f_shm_handler.initial_file_shm(mapped_file_pe_vec);

    // Call  Signature Engine.
    tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator> sig_engine;
    EXPECT_TRUE(sig_engine.create_engine(meta_sig_vec, utils::pe_file));

    // Register  Scanner Engine.
    std::string sigtype_code = utils::filetype_code_map(utils::pe_file);
    tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::register_actire_type(sigtype_code,
            tbbscan::actire_pe_engine<char, tbbscan::tbb_allocator>::create);

    // Create  Scanner Engine.
    tbbscan::iactire_engine<char, tbbscan::tbb_allocator> *iactire_concur =
            tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::create_actire_engine(sigtype_code);


    controller::tbbpostscan_pe_controller<controller::
    BufferSyncTBB<struct controller::data_tbb_process<struct MAPPED_FILE_PE>,
                struct MAPPED_FILE_PE>,
                    struct MAPPED_FILE_PE,
                        struct utils::meta_sig> tbbpostscan_pe_col;
    //Add Signature Engine to thread controller.
    tbbpostscan_pe_col.add_sig_engine(&sig_engine);

    //Add Search PE Engine to thread controller.
    tbbpostscan_pe_col.add_search_engine(iactire_concur);

    tbbpostscan_pe_col.init_syntbb_workload(f_shm_handler.get_map_str_shm(),
            &sig_shm_pe,
            f_shm_handler.get_map_file_size(),
            mapped_file_pe_vec);

    tbbpostscan_pe_col.task_start();
}

TEST_F(TBBPostScanPEController, MultitaskPerThreadScanning)
{


}

TEST_F(TBBPostScanPEController, MultitaskAndThreadScanning)
{


}
