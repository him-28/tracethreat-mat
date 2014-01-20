#include "utils/file_offset_handler.hpp"
//#include "filetypes/pe.hpp"

#define FILE_SIZE 1

using namespace util;

class FileOffsetHandlerTest : public ::testing::Test
{

    protected:
        virtual void SetUp() {
            char const *file_name_offset[FILE_SIZE];
            file_name_offset[0] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/clam-petite.exe";

            for(int count_file = 0; count_file < 	FILE_SIZE; count_file++) {
                list_file_type.push_back(file_name_offset[count_file]);

						    s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));
                vec_file_map_type.push_back(s_mapped_fpe[count_file]);

            }

        }
        std::list<std::string> list_file_type;
        struct MAPPED_FILE_PE *s_mapped_fpe[FILE_SIZE];
        std::vector<MAPPED_FILE_PE *> vec_file_map_type;

};

TEST_F(FileOffsetHandlerTest, mapped_file_test)
{
		file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;			
		h.mapped_file(list_file_type, mapped_vec, fileoffset_h); 
}; 
