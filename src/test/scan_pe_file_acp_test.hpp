#include "utils/file_offset_handler.hpp"
#include "scan/file_scan_policy.hpp"
#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"
#include "filetypes/pe_file_controller.hpp"
//#include "test/environment_test.hpp"

#define FILE_ON 4

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

            opencl_file_path    = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl/cl/tire_parallel.cl";
            file_sig            = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.hdb";

            for(int count_file = 0; count_file < 	FILE_ON; count_file++) {
                list_file_type.push_back(file_name_offset[count_file].c_str());
					
                //s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));
							 // struct MAPPED_FILE_PE * smf = &s_mapped_fpe[count_file];
								
                mapped_file_vec.push_back(&s_mapped_fpe[count_file]);
								/*
								smf->file_name.resize(file_name_offset[count_file].size());
								smf->file_name += std::string(file_name_offset[count_file]);
								std::cout<<"F : " << smf->file_name <<std::endl;
								*/

            }

						
            //td = new 	test_data[1];
            //td2 = new 	test_data[1];

            //load signature and input file

        }



        std::vector<const char *> list_file_type;
        struct MAPPED_FILE_PE s_mapped_fpe[FILE_ON];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;
        const char *file_sig;
        std::string file_name_offset[FILE_ON];

        //test with actire_parallel
        //typedef boost::unordered_map<char, size_t>  unordered_map;

        std::string  opencl_file_path;
        //test_data * td;
        //test_data * td2;

};


TEST_F(ScanACTireParallel, scan_file_policy_pe_type)
{

    typedef boost::unordered_map<char, size_t>  unordered_map;

    data_str::actire_parallel<char, size_t, unordered_map, std::vector<unordered_map> > acp;

    //typedef struct {
    std::vector<std::string> keywords;
    const char *input;
    //} test_data;

    keywords =  boost::assign::list_of("4c01b821685421cd70207369");
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

		for(int count = 0; count < 2; count++)
		{
			 struct MAPPED_FILE_PE * smf = mapped_file_vec[count];
			 std::cout<<" SMF : " << smf->file_name <<std::endl;
		}

    //    std::vector<struct MAPPED_FILE_PE *> * mapf_vec_ptr = fileoffset_h.get_mapped_file();
    //    typename std::vector<struct MAPPED_FILE_PE *>::iterator iter_mapf_vec_ptr;
		
    boost::shared_ptr<std::vector<MAPPED_FILE_PE * > > mappedf_vec_ptr =
            fileoffset_h.get_mappedf_vec_ptr();

    std::cout<<"scan_pe_file_acp_test, mappedf_vec_ptr.size() :  "<< mappedf_vec_ptr->size() <<std::endl;

    std::vector<MAPPED_FILE_PE *> *mapf_vec_ptr = mappedf_vec_ptr.get();

    typename std::vector<MAPPED_FILE_PE *>::iterator iter_mapf_vec;

    MAPPED_FILE_PE *mf_pe;

    for(iter_mapf_vec = mapf_vec_ptr->begin(); iter_mapf_vec != mapf_vec_ptr->end(); ++iter_mapf_vec) {
        mf_pe = *iter_mapf_vec;
        size_t data_size = mf_pe->size;
        EXPECT_GT(data_size,0);
    }
		
		
    std::cout<<"scan_pe_file_acp_test, mapf_vec_ptr size : " << mapf_vec_ptr->size() <<std::endl;
    //Scan step
    fpolicy::file_scan_policy<struct MAPPED_FILE_PE> *pef_policy =
            new fpolicy::pe_file_policy<struct MAPPED_FILE_PE>();
    // Scan_ocl_controller member function send vector symbol.
    pef_policy->scan_ocl_controller(symbol_shared_ptr.get(), state_shared_ptr.get());
    pef_policy->set_opencl_file_path(opencl_file_path);
    pef_policy->set_mapped_file(mapf_vec_ptr); //mf_pe

    fpolicy::scan_file_policy<
    MAPPED_FILE_PE,
    fpolicy::pe_policy_is<fpolicy::pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE>
    > sf_policy;
    sf_policy.scan_pe(pef_policy);
		
    EXPECT_TRUE(fileoffset_h.unmapped_file(mapped_file_vec));

    //clear
    //delete td[0].input;

}

/*
GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment( new SystemTestingEnvironment(argc, argv));
    return RUN_ALL_TESTS();
}
*/
