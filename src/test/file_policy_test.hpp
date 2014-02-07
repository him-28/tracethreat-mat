
#include "scan/file_scan_policy.hpp"
#include "scan/pe_file_policy.hpp"

class FilePolicyTest : public ::testing::Test
{
        virtual void SetUp() {

            char const *file_name_offset[FILE_SIZE];

            file_name_offset[0] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/Crack.exe";

            for(int count_file = 0; count_file < 	FILE_SIZE; count_file++) {
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


};
