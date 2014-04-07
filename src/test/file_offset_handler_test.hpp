#include "utils/file_offset_handler.hpp"
//#include "filetypes/pe.hpp"

#define FILE_SIZE 1

using namespace utils;

class FileOffsetHandlerTest : public ::testing::Test
{

    protected:
        virtual void SetUp() {
            char const *file_name_offset[FILE_SIZE];
            file_name_offset[0] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/clam-petite.exe";

            for(int count_file = 0; count_file < 	FILE_SIZE; count_file++) {
                list_file_type.push_back(file_name_offset[count_file]);

						    s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));
								/*s_mapped_fpe[count_file]->file_name = 
													(const char*)malloc(sizeof(const char*) * strlen(file_name_offset[0]));*/
                mapped_file_vec.push_back(s_mapped_fpe[count_file]);

            }

        }
        std::vector<const char*> list_file_type;
        struct MAPPED_FILE_PE *s_mapped_fpe[FILE_SIZE];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;

};

/**
* @brief mapped_file_test, Mapped file to memory and get data and size of data.
*
* @param FileOffsetHandlerTest  Class initial variable when SetUp() setup value to variable.
* @param mapped_file_test  Test file mapped to memory. Mapped file support large file on operating system.
*/
TEST_F(FileOffsetHandlerTest, mapped_file_test)
{
		file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;			

		EXPECT_TRUE(fileoffset_h.mapped_file(list_file_type, mapped_file_vec, fileoffset_h));

		//std::vector<MAPPED_FILE_PE*> * mapped_file_vec_ptr = fileoffset_h.get_mapped_file();

    boost::shared_ptr<std::vector<MAPPED_FILE_PE * > > mappedf_vec_ptr =
            fileoffset_h.get_mappedf_vec_ptr();

    std::vector<MAPPED_FILE_PE *> *mapped_file_vec_ptr = mappedf_vec_ptr.get();


		typename std::vector<MAPPED_FILE_PE*>::iterator iter_mapped_file;
		for(iter_mapped_file = mapped_file_vec_ptr->begin(); 
				iter_mapped_file != mapped_file_vec_ptr->end(); 
				++iter_mapped_file)
		{
							MAPPED_FILE_PE * mf_pe = *iter_mapped_file;
							unsigned char * data = mf_pe->data;
							size_t size  = mf_pe->size; 
						  EXPECT_GT(size,0);
							ASSERT_TRUE(*data != NULL);
/*
							for(size_t count = 0; count < size; count++)
							{
							   printf("%x",data[count]); 
							}	
							printf("\n");
*/
		}

    EXPECT_TRUE(fileoffset_h.unmapped_file(mapped_file_vec));

};
 
