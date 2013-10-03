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
								std::string path      = std::string(file_path).append(std::string("/"));
							 	std::string full_path = std::string(path).append(std::string(ent->d_name));
								files.push_back(full_path);
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

