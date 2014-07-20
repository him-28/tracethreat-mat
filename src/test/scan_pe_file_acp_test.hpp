#include "utils/file_offset_handler.hpp"
#include "scan/file_scan_policy.hpp"

#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"

#include "filetypes/pe_file_controller.hpp"

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"

#include "threadconcurrency/tbbpostscan_pe_controller.hpp"
#include "tbb/concurrent_vector.h"
#include "utils/base/system_code.hpp"


#include "memory/file_shm_handler.hpp"
#include "filetypes/pe.hpp"


#include "utils/base/common.hpp"


namespace data_str    = data_structure;
namespace fpolicy = policy;

using namespace utils;
using namespace memory;




class ScanACTireParallel : public ::testing::Test
{

    protected:



        virtual void SetUp() {
            //load binary file pe.
            f_handler.file_full_path("/home/chatsiri/sda1/workspacemalware/malware_debug/pack_test_case_01");
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
            }

            //support signature base scanning.
            //load binary file pe.
            meta_sig_vec.push_back(new struct utils::meta_sig);
            msig = meta_sig_vec[0];
            msig->sig      = "ab09cd21b8014ccd215468\0"; //not should ab
            msig->virname = "Trojan-Case-01\0";
            msig->sig_type = utils::pe_file;
            msig->offset   = 140;
            msig->sig_size = strlen(msig->sig);
            msig->sig_detail = "Trojan for testing-01.\0";

            meta_sig_vec.push_back(new struct utils::meta_sig);
            msig = meta_sig_vec[1];
            msig->sig  = "4d5a50000200000004000f00ffff0000b80000000000000040001a000000";
            msig->virname   = "trojan-zbot-1563";
            msig->sig_type  = utils::pe_file;
            msig->sig_size  = strlen(msig->sig);
            msig->sig_detail = "Trojan for test-zebot\0";


        }


        //read file.
        std::vector<const char *> file_type_vec;
        struct MAPPED_FILE_PE *s_mapped_fpe;
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;
        const char *file_sig;
        std::string file_name_offset[FILE_ON];
        std::string  opencl_file_path;

        //read file handler
        utils::file_handler<struct common_filetype> f_handler;


        //support shm pe
        std::vector<struct utils::meta_sig *>  meta_sig_vec;

        struct utils::meta_sig *msig;
        uint8_t sig_type;
        //Output function.
        tbb::concurrent_unordered_map<std::size_t,
            std::set<struct utils::meta_sig *> > output_fn;



};


TEST_F(ScanACTireParallel, scan_file_policy_pe_type)
{
    std::string shm_name = "shm-pe";

    typedef boost::unordered_map<char, size_t>  unordered_map;

    data_str::actire_parallel<char, size_t, unordered_map, std::vector<unordered_map> > acp;

    std::vector<std::string> keywords;
    const char *input;

    //keywords =  boost::assign::list_of("4c01b821685421cd70207369");

    //keywords =  boost::assign::list_of("09cd21b8014ccd215468"); //support c++ standard

    keywords =  {"09cd21b8014ccd215468"}; //support c++0x

    //("8d2e1a0abb1a")("cc8de2b1");

    //boost::assign::list_of("4bc647")("4b537db2e3a337")("4b53abf");
    input    = (char *)malloc(sizeof(char *) * std::string("a824bc647c46d04b537db2e3a33711").size());
    input    = "a824bc647c46d04b537db2e3a33711";


    char *input_tmp = const_cast<char *>(input);
    char *end_input_tmp = const_cast<char *>(&input[strlen(input)]);
    data_str::ac_graph<char> ac(keywords.begin(), keywords.end());
    acp.prepare_graph(&ac.get_graph());


    std::vector<char>   nsymbol;
    std::vector<size_t> nstate;
    boost::shared_ptr<std::vector<char> >   symbol_shared_ptr  = acp.get_symbol_shared_ptr();
    boost::shared_ptr<std::vector<size_t> >  state_shared_ptr = acp.get_state_shared_ptr();

    // Second, Send Symbol and State vector to  file_scan_policy
    // list_file_tye insert file name, s_mapped_fpe inserted  file_type details.
    utils::file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;

    EXPECT_TRUE(fileoffset_h.mapped_file(&file_type_vec, &mapped_file_vec, fileoffset_h));


    boost::shared_ptr<std::vector<MAPPED_FILE_PE * > > mappedf_vec_ptr =
            fileoffset_h.get_mappedf_vec_ptr();


    std::vector<MAPPED_FILE_PE *> *mapf_vec_ptr = mappedf_vec_ptr.get();

    typename std::vector<MAPPED_FILE_PE *>::iterator iter_mapf_vec;

    MAPPED_FILE_PE *mf_pe;

    for(iter_mapf_vec = mapf_vec_ptr->begin(); iter_mapf_vec != mapf_vec_ptr->end(); ++iter_mapf_vec) {
        mf_pe = *iter_mapf_vec;
        size_t data_size = mf_pe->size;
        EXPECT_GT(data_size,0);
    }


    memory::signature_shm_pe_controller<struct memory::meta_sig, struct memory::meta_sig_mem>
                sig_shm_pe;



    //Initial engine.
    // Call  Signature Engine.
    tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator> sig_engine;

    EXPECT_TRUE(sig_engine.create_engine(meta_sig_vec, utils::pe_file));

    // Register  Scanner Engine.
    std::string sigtype_code = utils::filetype_code_map(utils::pe_file);

    tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::register_actire_type(sigtype_code,
            tbbscan::actire_pe_engine<char, tbbscan::tbb_allocator>::create);

    // Create  Scanner Engine.
    tbbscan::iactire_engine<char, tbbscan::tbb_allocator> *iactire_engine_scanner =
            tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::create_actire_engine(sigtype_code);


    //Scan step
    fpolicy::file_scan_policy<struct MAPPED_FILE_PE> *pef_policy =
            new fpolicy::pe_file_policy<struct MAPPED_FILE_PE>();

    // Scan_ocl_controller member function send vector symbol.
    pef_policy->set_sig_buffer(symbol_shared_ptr.get(), state_shared_ptr.get());

    pef_policy->set_opencl_file_path(opencl_file_path);

    pef_policy->set_mapped_file(mapf_vec_ptr); //mf_pe

    //signature initial to map (Plant-00004: SHM support )
    sig_shm_pe.initial_shm_sigtype(&meta_sig_vec, shm_name);

    fpolicy::scan_file_policy<
    MAPPED_FILE_PE,
    fpolicy::pe_policy_is<fpolicy::pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE>
    > sf_policy;

    sf_policy.scan_pe(pef_policy,&sig_shm_pe, &sig_engine, iactire_engine_scanner);

    EXPECT_TRUE(fileoffset_h.unmapped_file(mapped_file_vec));

}

