#include "utils/file_offset_handler.hpp"
#include "scan/file_scan_policy.hpp"

#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"

#include "filetypes/pe_file_controller.hpp"

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"


#include "utils/base/common.hpp"

#define FILE_ON 4
#define VIRUS_SIG  1

namespace data_str    = data_structure;
namespace fpolicy = policy;
using namespace utils;//   = util;




class ScanACTireParallel : public ::testing::Test
{

    protected:



        virtual void SetUp() {
            //load binary file pe.
            file_name_offset[0] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext.exe";
            file_name_offset[1] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_01.exe";

            file_name_offset[2] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_02.exe";

            file_name_offset[3] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_03.exe";
            /*        file_name_offset[4] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_04.exe";
                    file_name_offset[5] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_05.exe";
                    file_name_offset[6] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_06.exe";
                    file_name_offset[7] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_07.exe";
                    file_name_offset[8] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_08.exe";
                    file_name_offset[9] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_09.exe";
                    file_name_offset[10] = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_ext_10.exe";
            */

            opencl_file_path    = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl/cl/tire_parallel.cl";
            file_sig            = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.hdb";

            for(int count_file = 0; count_file < 	FILE_ON; count_file++) {
                list_file_type.push_back(file_name_offset[count_file].c_str());

                mapped_file_vec.push_back(&s_mapped_fpe[count_file]);

            }

            //support signature base scanning.
            //load binary file pe.
            struct utils::meta_sig *msig;
            msig = &meta_sig_def[0];
            msig->sig_type = utils::pe_file;
            msig->offset   = 140;
            msig->sig      = "09cd21b8014ccd215468\0";
            msig->virname = "Trojan-Case-01\0";
            msig->sig_detail = "Trojan for testing-01.\0";
            meta_sig_vec.push_back(&meta_sig_def[0]);

        }


        //read file.
        std::vector<const char *> list_file_type;
        struct MAPPED_FILE_PE s_mapped_fpe[FILE_ON];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;
        const char *file_sig;
        std::string file_name_offset[FILE_ON];
        std::string  opencl_file_path;

        //support shm pe
        struct utils::meta_sig meta_sig_def[VIRUS_SIG];
        std::vector<struct utils::meta_sig *>  meta_sig_vec;
        memory::signature_shm_pe_controller<struct utils::meta_sig, struct memory::meta_sig_mem>
                    sig_shm_pe;

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


    /*
    for(int i = 0; i  < 1; i++) {
    char *input_tmp = const_cast<char *>(&td[i].input[0]);
    char *end_input_tmp = const_cast<char *>(&td[i].input[strlen(td[i].input)]);
    data_str::ac_graph<char> ac(td[i].keywords.begin(),td[i].keywords.end());
    acp.prepare_graph(&ac.get_graph());
    }
    */

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
    EXPECT_TRUE(fileoffset_h.mapped_file(list_file_type, mapped_file_vec, fileoffset_h, file_sig));

    for(int count = 0; count < 2; count++) {
        struct MAPPED_FILE_PE *smf = mapped_file_vec[count];
        std::cout<<" SMF : " << smf->file_name <<std::endl;
    }

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


    //Scan step
    fpolicy::file_scan_policy<struct MAPPED_FILE_PE> *pef_policy =
            new fpolicy::pe_file_policy<struct MAPPED_FILE_PE>();
    // Scan_ocl_controller member function send vector symbol.
    pef_policy->scan_ocl_controller(symbol_shared_ptr.get(), state_shared_ptr.get());
    pef_policy->set_opencl_file_path(opencl_file_path);
    pef_policy->set_mapped_file(mapf_vec_ptr); //mf_pe

    //signature initial to map (Plant-00004: SHM support )
    sig_shm_pe.initial_shm_sigtype(&meta_sig_vec, shm_name);

    fpolicy::scan_file_policy<
    MAPPED_FILE_PE,
    fpolicy::pe_policy_is<fpolicy::pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE>
    > sf_policy;
    sf_policy.scan_pe(pef_policy,&sig_shm_pe);

    EXPECT_TRUE(fileoffset_h.unmapped_file(mapped_file_vec));

}

