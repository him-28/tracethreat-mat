#include "file_calculate.hpp"
/*                       Titles                                          Authors                        Date
 * - Get fil and processes file size                                     R.Chatsiri
 */

namespace util
{

    template<typename Extension>
    bool file_calculate<Extension>::processes()
    {
        if((dir = opendir(file_path)) != NULL) {
            while((ent = readdir(dir)) != NULL) {
                std::cout<<" Path file name : " << ent->d_name <<std::endl;
            }
            closedir(dir);
        } else {
            return false;
        }

        return true;
    }
		template<typename Extension>
		bool file_calculate<Extension>::set_filepath(char * file_path)
		{
				this->file_path = file_path;
		}
	  template class file_calculate<Extension>;

}

