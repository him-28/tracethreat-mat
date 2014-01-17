#include "file_offset_handler.hpp"
template<typename FileType, typename MAPPED_FILE>
void  file_offset_handler<FileType, MAPPED_FILE>::file_offset_handler()
{

    //logger
    logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
    logger = logger_ptr->get();
    logger->write_info_test("Init logger fileoffset");
}


template<typename FileType,  typename MAPPED_FILE>
bool file_offset_handler<FileType, MAPPED_FILE>::set_file_offset(typename FileType::filename *p_file)
{


}

template<typename FileType,  typename MAPPED_FILE>
std::vector<pack_file_offset> file_offset_handler<FileType, MAPPED_FILE>::offset_perfile()const
{


}


template<typename FileType, typename MAPPED_FILE>
void  file_offset_handler<FileType, MAPPED_FILE>::mapped_file(std::list<std::string> file_name_list,
        std::vector<MAPPED_FILE *> mapped_vec)
{
    std::list<std::string>::iterator iter_files;
    boost::shared_ptr<std::vector<MAPPED_FILE *> > mapped_file_vec_shared_ptr(mapped_vec_);
    mapped_vec_  = mapped_vec;

    for(iter_files = file_name_list.begin();
            iter_files != file_name_list.end();
            ++iter_files) {

        try {
            if(*iter_file == NULL)
                throw file_system_excep::offset_exception("[** File is null **]");

            const char *file_name  = *iter_files.c_str();

        } catch(file_system_excep::offset_exception& offset_excep) {
            logger->write_info("Error, file offset", offset_excep.message_error_file_is_null());
            continue;
        }

        if(ifh->set_filepath(*ite_file)) {
            mapped_file_ptr =	mapped_vec_[iter_files - file_name_list.begin()];

            try {
                if(mapped_file_ptr == NULL) {
                    throw file_system_excep::offset_exception("[** Mapped pointer is null. **]");
                }

                status *file_status =  get_file_status();
                mapped_file_ptr->size =	file_status.st_size;

                if(mapped_file_ptr->size == 0) {
                    throw file_system_excep::offset_exception("[** File size don't get status **]");
                    continue;
                }

                mapped_file_ptr->data = (uint8_t *)mmap(0,
                        mapped_file_ptr->size,
                        PROT_READ,
                        MAP_PRIVATE,
                        mapped_file_ptr->file,
                        0);

                if(mapped_file_ptr->data == MAP_FAILED) {
                    throw file_system_excep::offset_exception("[** File cannot map **]");
                    continue;
                }

            } catch(file_system_excep::offset_exception& offset_excep) {

                logger->write_info("Error, Mapped file", offset_excep.message_error_file_is_null());
            }
        }
    }

    template<typename FileType, typename MAPPED_FILE>
    bool  file_offset_handler<FileType, MAPPED_FILE>::unmapped_file(std::vector<MAPPED_FILE * > mapped_vec) {
        std::vector<MAPPED_FILE *>::iterator iter_mapped_files;

        for(iter_mapped_files = mapped_vec.begin();
                iter_mapped_files != mapped_vec.end();
                ++iter_mapped_files) {
            struct MAPPED_FILE *mapped_file_ptr = *iter_mapped_files;
            munmap(mapped_file_ptr->data, mapped_file_ptr->size);
        }

    }


}

