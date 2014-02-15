#include "filetypes/pe_file_controller.hpp"
#include "utils/file_offset_handler.hpp"

#define FILE_SIZE 1

using namespace util;
using namespace filetypes;

enum error_code { SUCCESS_COMPLETED_FILE, ERROR_CANNOT_OPEN, ERROR_PE_CORRUPT_FILE };

class PEFileControllerTest : public ::testing::Test
{
    protected:
        virtual void SetUp() {

            char const *file_name_offset[FILE_SIZE];

           // file_name_offset[0] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/Crack.exe";
            file_name_offset[0] = "/home/chatsiri/sda1/workspacemalware/lab_malwareanalysis/3/clam_ISmsi_ext.exe";

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

    file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;
    fileoffset_h.mapped_file(list_file_type, mapped_file_vec, fileoffset_h);

    std::vector<struct MAPPED_FILE_PE *> pe_header_vec_ptr = fileoffset_h.get_mapped_file();

    pe_file_controller<struct MAPPED_FILE_PE>  pe_file_col;
		pe_file_col.get_pe_header(&pe_header_vec_ptr);

    typename std::vector<struct IMAGE_NT_HEADERS *>::iterator iter_pe_header;
/*
    for(iter_pe_header = pe_header_shared_ptr->begin();
            iter_pe_header != pe_header_shared_ptr->end();
            ++iter_pe_header) {
        pe_file_col.list_pe_header(*iter_pe_header);
    }
*/
};

