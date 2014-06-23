#include "scan/file_scan_policy.hpp"
#include "utils/file_offset_handler.hpp"
//#include "threadconcurrency/cliprescan_pe_controller.hpp"
#include "test/environment_test.hpp"

#define FILE_SIZE_POLICY 1
class FilePolicyTest : public ::testing::Test
{
    protected:
        virtual void SetUp() {


            opencl_file_path    =
                    "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/ocl/cl/tire_parallel.cl";

            //file_name_offset[0]    =
            //            "/home/chatsiri/sda1/workspacemalware/lab_malwareanalysis/3/yara_scan_old.sh";
						/*
            file_name_offset[0] =
                    "/home/chatsiri/sda1/workspacemalware/lab_malwareanalysis/3/clam_ISmsi_ext.exe";
						*/
            file_name_offset[0] =
                    "/home/chatsiri/sda1/workspacemalware/malware_debug/infected_01.exe";

						file_sig = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.ndb";

            for(int count_file = 0; count_file < 	FILE_SIZE_POLICY; count_file++) {
                file_type_vec.push_back(file_name_offset[count_file]);

                s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));
								
								struct MAPPED_FILE_PE * sm = s_mapped_fpe[count_file];

															
               mapped_file_vec.push_back(s_mapped_fpe[count_file]);

            }


        }

        const char *file_name_offset[FILE_SIZE_POLICY];
        std::vector<const char *> file_type_vec;
        struct MAPPED_FILE_PE *s_mapped_fpe[FILE_SIZE_POLICY];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;
        std::string opencl_file_path;
				const char * file_sig;
};


TEST_F(FilePolicyTest, pe_file_policy)
{

    using namespace policy;

    utils::file_offset_handler<struct utils::common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;

    EXPECT_TRUE(fileoffset_h.mapped_file(file_type_vec, mapped_file_vec, fileoffset_h, file_sig));

    uint64_t sum_file_size = 0;

    boost::shared_ptr<std::vector<MAPPED_FILE_PE * > > mappedf_vec_ptr =
            fileoffset_h.get_mappedf_vec_ptr();

    std::vector<MAPPED_FILE_PE *> *mapped_file_vec_ptr = mappedf_vec_ptr.get();

    typename std::vector<MAPPED_FILE_PE *>::iterator iter_mapped_file;

    for(iter_mapped_file = mapped_file_vec_ptr->begin();
            iter_mapped_file != mapped_file_vec_ptr->end();
            ++iter_mapped_file) {

        MAPPED_FILE_PE *mf_pe = *iter_mapped_file;
        unsigned char *data = mf_pe->data;
        EXPECT_GT(mf_pe->size,0);

        /*logger->write_info_test("Loop sync : mf_pe->size",
        		boost::lexical_cast<std::string>(mf_pe->size));
        logger->write_info_test("Loop sync : mf_pe->filename",
        		boost::lexical_cast<std::string>(mf_pe->file_name));
				*/

        sum_file_size += mf_pe->size;
        ASSERT_TRUE(*data != NULL);
				std::cout<<" Summary_file : " << sum_file_size <<std::endl;
				std::cout<<" File name : " << sum_file_size <<std::endl;
    }


    //Assert

    //pe_file_policy<struct MAPPED_FILE_PE>  pef_policy;// =  new pe_file_policy<struct MAPPED_FILE_PE>();
    //4d5a9000030000000400";
    const char str1[] = "e1fba0e00b409cd21b8014ccd215468";
    std::vector<char>  nsymbol;
    nsymbol.insert(nsymbol.end(), str1, str1 + strlen(str1));
    printf("nsymbol size : %d \n", nsymbol.size());

    std::vector<size_t> nstate;
		for(int count_state = 0; count_state < 30; count_state++)
		{
				nstate.push_back(count_state);
		}
    //nstate.push_back(1); // test state only
    printf("nstate size : %d \n", nstate.size());

    file_scan_policy<struct MAPPED_FILE_PE> *pef_policy = new pe_file_policy<struct MAPPED_FILE_PE>();
    //get signature base.
    pef_policy->scan_ocl_controller(&nsymbol, &nstate);

    pef_policy->set_opencl_file_path(opencl_file_path);

    //get mapped file.
    pef_policy->set_mapped_file(mapped_file_vec_ptr);

    scan_file_policy<
    MAPPED_FILE_PE,
    pe_policy_is<pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE>
    > sf_policy;

    sf_policy.scan_pe(pef_policy);

    /*
    EXPECT_TRUE(fileoffset_h.unmapped_file(*mapped_file_vec_ptr));
    */

};

GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment( new SystemTestingEnvironment(argc, argv));
    return RUN_ALL_TESTS();
}
