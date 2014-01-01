#include "file_handler.hpp"
#include <stdexcept>
#include <stdio.h>

namespace util
{

    template<typename FileType>
    bool file_handler<FileType>::file_read()
    {
        p_file = fopen(file_path,"r");

        if(p_file == NULL) {
            throw std::runtime_error("File cannot open");
            return false;
        }

        return true;
    }


    template<typename FileType>
    bool file_handler<FileType>::set_filepath(char const *file_path)
    {
        this->file_path = file_path;

        if(this->file_path != NULL)
            return true;

        return false;
    }

    template<typename FileType>
    typename FileType::filename *file_handler<FileType>::get_file() const
    {
        return p_file;
    }

    template<typename FileType>
    bool file_handler<FileType>::get_fdetail_create()
    {
        //    if(file_d)  return true;

        return false;
    }

    template class file_handler<common_filetype>;

}
