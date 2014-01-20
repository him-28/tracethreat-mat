#include "utils/file_offset_handler.hpp"
#include "filetypes/pe.hpp"

#define FILE_SIZE 1;

class FileOffsetHandlerTest : public ::testing::Test
{

		protected:
			virtual void SetUp(){
						
			}
			std::list<std::string> list_file_type;
		  const char file_name[1] = { 
					"/home/chatsiri/sda1/workspacecpp/clamav-devel/test/clam-petite.exe"
			};	
			
			for(int count_file = 0; count_file < 	FILE_SIZE; count_file++)
			{
						list_file_type.push_back(file_name[count_file]);
			}
	
};
