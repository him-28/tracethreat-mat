#include "file_calculate.hpp"
/*                       Titles                                          Authors                        Date
 * - Get fil and processes file size                                     R.Chatsiri
 */

namespace util
{

    template<typename Extension>
    bool file_calculate<Extension>::processes()
    {
        char current_dir[]  = ".";
        char local_dir[]    = "..";

        if((dir = opendir(file_path)) != NULL) {
            while((ent = readdir(dir)) != NULL) {
                std::string path      = std::string(file_path).append(std::string("/"));

                if(strcmp(ent->d_name, current_dir) == 0 || strcmp(ent->d_name, local_dir) == 0) {
                    //EXPECT_EQ(0, strcmp(ent->d_name, current_dir));
                    continue;
                }

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
    bool file_calculate<Extension>::set_filepath(char *file_path)
    {
        this->file_path = file_path;
    }

    // use with C code
    template<typename Extension>
    unsigned int file_calculate<Extension>::file_size()
    {
        long int size_summary = 0;
        unsigned int hex2size_int = 0;
        size = 0;

				file_d = (struct file_detail *)malloc(sizeof(file_d) * MAX_FILE_INCLUDED);
					
        for(typename std::list<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter, count_file++) {
            std::stringstream ss;
					
            file_name = (*iter).c_str();
            p_file    = fopen(file_name, "rb");
            fseek(p_file, 0, SEEK_END);
 
            file_d[count_file].size_cal = (unsigned int*)ftell(p_file);
	
            file_d[count_file].file_cal = file_name;

            fclose(p_file);
        }

    }

    template<typename Extension>
    unsigned int file_calculate<Extension>::get_count_file()
    {
        return count_file;
    }

    template<typename Extension>
    struct file_detail *file_calculate<Extension>::get_file_d() {
        return file_d;
    }
    template<typename Extension>
    file_calculate<Extension>::~file_calculate()
    {
        /*
        	delete p_file;
        	delete file_path;
        	delete ent;
        	delete file_cal;
        	delete name_cal;
        */
    }

    template class file_calculate<Extension>;

}

