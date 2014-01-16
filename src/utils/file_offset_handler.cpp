#include "file_offset_handler.hpp"

template<typename FileType,  typename MAPPED_FILE>
bool file_offset_handler<FileType, MAPPED_FILE>::set_file_offset(typename FileType::filename *p_file)
{


}

template<typename FileType,  typename MAPPED_FILE>
std::vector<pack_file_offset> file_offset_handler<FileType, MAPPED_FILE>::offset_perfile()const
{


}


template<typename FileType, typename MAPPED_FILE>
void  file_offset_handler<FileType, MAPPED_FILE>::mapped_file(std::list<std::string> file_name,
        std::vector<MAPPED_FILE *> mapped_vec)
{
    std::list<std::string>::iterator iter_files;
    std::vector<MAPPED_FILE *>::iterator iter_mapped_files;
    boost::shared_ptr<std::vector<MAPPED_FILE *> > mapped_file_vec_shared_ptr(mapped_vec_);

    for(iter_files = file_name.begin();
            iter_file != file_name.end();
            ++iter_file) {

        try {


        } catch() {


        }

        if(set_filepath(*ite_file)) {

        }

        for(iter_mapped_files = mapped_vec.begin();
                iter_mapped_files != mapped_vec.end();
                ++iter_mapped_files) {

        }
    }

}

