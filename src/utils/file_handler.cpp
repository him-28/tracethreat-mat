#include "file_handler.hpp"

namespace utils
{

    template<typename FileType>
    bool file_handler<FileType>::file_read()
    {

        if(stat(file_path, &file_status) != 0 || S_ISDIR(file_status.st_mode)) {
            throw std::runtime_error("File cannot check status");
            return false;
        }

        p_file = fopen(file_path,"r");

        if(p_file == NULL) {
            throw std::runtime_error("File cannot open");
            return false;
        }

        return true;
    }
    template<typename FileType>
    bool file_handler<FileType>::file_read_mapped()
    {

        if(stat(file_path, &file_status) != 0 || S_ISDIR(file_status.st_mode)) {
            throw std::runtime_error("File cannot check status");
            return false;
        }

        p_open_file = open(file_path,O_RDONLY);

        if(p_open_file == NULL) {
            throw std::runtime_error("File cannot open");
            return false;
        }

        return true;
    }

    template<typename FileType>
    typename FileType::file_open_ptr &  file_handler<FileType>::get_popen_file()
		{
				return p_open_file;
		}


    template<typename FileType>
    struct stat    *file_handler<FileType>::get_file_status() {
        return &file_status;
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
    typename FileType::file_ptr *file_handler<FileType>::get_file() const
    {
        return p_file;
    }

    template<typename FileType>
    bool file_handler<FileType>::get_fdetail_create()
    {
        //    if(file_d)  return true;

        return false;
    }

    template<typename FileType>
    bool file_handler<FileType>::close_file()
    {
        close(p_open_file);
        return true;
    }

    template class file_handler<common_filetype>;
    //template class file_handler<common_openfile_type>;

    //file_strem
    template<typename FileType,typename StructFileType, typename PointerType>
    bool file_stream_handler<FileType,StructFileType, PointerType>::file_read()
    {
        file_stream_read.open(this->file_path);

        if(file_stream_read.good()) {

        }

        return true;
    }


    template<typename FileType,typename StructFileType, typename PointerType>
    bool file_stream_handler<FileType,StructFileType, PointerType>::set_filepath(char const *file_path)
    {
        this->file_path = file_path;

        if(this->file_path != NULL)
            return true;

        return false;
    }

    /*
    template<typename FileType,typename StructFileType, typename PointerType>
    typename  FileType::file_ptr * file_stream_handler<FileType, StructFileType, PointerType>
    ::get_file() const
    {
    return const_cast<typename FileType::file_ptr *>(p2file); // pointer to file description
    }
    */

    template class file_stream_handler<struct MAPPED_FILE_PE, char,
             struct common_stream_filetype<struct MAPPED_FILE_PE, char> >;



}
