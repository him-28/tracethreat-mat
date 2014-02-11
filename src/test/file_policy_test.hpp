
#include "scan/file_scan_policy.hpp"
#include "scan/pe_file_policy.hpp"

#define FILE_NO 1

class FilePolicyTest : public ::testing::Test
{
				protected:
        virtual void SetUp() {

            char const *file_name_offset[FILE_SIZE];

            file_name_offset[0] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/Crack.exe";

            for(int count_file = 0; count_file < 	FILE_NO; count_file++) {
                list_file_type.push_back(file_name_offset[count_file]);

                s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));

                mapped_file_vec.push_back(s_mapped_fpe[count_file]);

            }


        }
        std::list<std::string> list_file_type;
        struct MAPPED_FILE_PE *s_mapped_fpe[FILE_SIZE];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;

};


TEST_F(FilePolicyTest, pe_file_policy)
{
		using namespace policy;
    // list_file_tye insert file name, s_mapped_fpe inserted  file_type details.
    file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;
    EXPECT_TRUE(fileoffset_h.mapped_file(list_file_type, mapped_file_vec, fileoffset_h));

    std::vector<struct MAPPED_FILE_PE *> mapf_vec = fileoffset_h.get_mapped_file();
    typename std::vector<struct MAPPED_FILE_PE *>::iterator iter_mapf_vec;

    for(iter_mapf_vec = mapf_vec.begin(); iter_mapf_vec != mapf_vec.end(); ++iter_mapf_vec) {
				MAPPED_FILE_PE * mf_pe  = *iter_mapf_vec;
				size_t data_size = mf_pe->size;
				EXPECT_GT(data_size,0);
				
				//send to pe get header
				
				
        // offset for scanning data

    }
	  pe_file_policy<struct MAPPED_FILE_PE> * pef_policy;// =  new pe_file_policy<struct MAPPED_FILE_PE>();	
    scan_file_policy<
				MAPPED_FILE_PE,
				pe_policy_is<pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE> 
				> sf_policy;
    sf_policy.scan_pe(pef_policy);

    EXPECT_TRUE(fileoffset_h.unmapped_file(mapped_file_vec));

};

