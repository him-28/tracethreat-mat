#include "file_calculate.hpp"
/*                       Titles                                          Authors                        Date
 * - Get fil and processes file size                                     R.Chatsiri
 */

namespace util
{

    int compare_function(const void *valuel, const void *valuer)
    {
        struct file_detail *valuel_ = (file_detail *)valuel;
        unsigned int size_l = valuel_->size_cal;
        struct file_detail *valuer_ = (file_detail *)valuer;
        unsigned int size_r = valuer_->size_cal;
        return size_l - size_r;
    }


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
    bool file_calculate<Extension>::set_filepath(const char *file_path)
    {
        this->file_path = file_path;
				if(this->file_path != NULL)
				  return true;
				return false;
    }

    // use with C code
    template<typename Extension>
    unsigned int file_calculate<Extension>::file_size()
    {
        long int size_summary = 0;
        unsigned int hex2size_int = 0;
        size = 0;
        count_file = 0;
        file_d = (struct file_detail *)malloc(sizeof(file_d) * MAX_FILE_INCLUDED);

        for(typename std::list<std::string>::iterator iter = files.begin();
                iter != files.end();
                ++iter, count_file++) {
            std::stringstream ss;

            file_name = (*iter).c_str();
            p_file    = fopen(file_name, "rb");
            fseek(p_file, 0, SEEK_END);

            ss << std::hex << (unsigned int)ftell(p_file);
            ss >> hex2size_int;
            file_d[count_file].size_cal = hex2size_int;// (unsigned int)ftell(p_file);
            file_d[count_file].file_cal = file_name;
            fclose(p_file);
        }

        buffer_size = 0;
        qsort(file_d, count_file, sizeof(*file_d), compare_function);

        for(int count_filter = 0; count_filter < count_file; count_filter++) {
            if(buffer_size > MAX_BUFFER_SIZES)
                break;

            buffer_size += file_d[count_filter].size_cal;
            files2buffer.push_back(file_d[count_filter].file_cal);
        }
			return buffer_size; // change to size of file on list
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
    std::list<std::string>& file_calculate<Extension>::get_files2buffer()
    {		
				std::cout<<" file 2 buffer : " << files2buffer.size() <<std::endl;
        return files2buffer;
    }

    template<typename Extension>
    file_calculate<Extension>::~file_calculate()
    {
        file_name = NULL;
        file_path = NULL;
        //dir       = NULL;
        ent       = NULL;
        file_d    = NULL;
        p_file    = NULL;
        delete file_name;
        delete file_path;
        //delete dir;
        delete file_d;
        delete p_file;
        delete ent;
    }

    template class file_calculate<Extension>;

}

