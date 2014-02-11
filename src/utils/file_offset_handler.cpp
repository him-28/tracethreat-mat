
#include <iostream>

#include "file_offset_handler.hpp"

namespace util
{


    template<typename FileType, typename MAPPED_FILE>
    file_offset_handler<FileType, MAPPED_FILE>::file_offset_handler()
    {

        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        logger->write_info_test("Init logger fileoffset");
    }


    template<typename FileType,  typename MAPPED_FILE>
    bool file_offset_handler<FileType, MAPPED_FILE>::set_file_offset(typename FileType::file_ptr *p_file)
    {


    }

    template<typename FileType,  typename MAPPED_FILE>
    std::vector<pack_file_offset<FileType> > file_offset_handler<FileType, MAPPED_FILE>::offset_perfile()const
    {


    }


    template<typename FileType, typename MAPPED_FILE>
    bool file_offset_handler<FileType, MAPPED_FILE>::mapped_file(std::list<std::string> file_name_list,
            std::vector<MAPPED_FILE *> mapped_vec,
            file_offset_handler<FileType, MAPPED_FILE>& file_offset_object)
    {
        std::list<std::string>::iterator iter_files;

        mapped_vec_shared = boost::make_shared<std::vector<MAPPED_FILE * > >();
        mapped_vec_shared->swap(mapped_vec);
        //mapped_file_vec_shared.push_back(mapped_vec_shared);

        const char *file_name;
        MAPPED_FILE *mapped_file_ptr;

        if(!file_name_list.size() || !mapped_vec_shared->size() ) {
            logger->write_info("Not data on file_name or mapped file");
            return false;
        }

        for(iter_files = file_name_list.begin();
                iter_files != file_name_list.end();
                ++iter_files) {

            try {
                std::string s_file_name = *iter_files;

                if(s_file_name.empty())
                    throw file_system_excep::offset_exception("[** File is null **]");

                logger->write_info("File path for mmaped ", s_file_name);

                file_name  = s_file_name.c_str();

            } catch(file_system_excep::offset_exception& offset_excep) {
                logger->write_info("Error, file offset", offset_excep.message_error_file_is_null());
                continue;
            }

            if(file_offset_object.set_filepath(file_name)) {

                if(!file_offset_object.file_read_mapped()) {
                    throw file_system_excep::offset_exception("[** File cannot open path **]");
                }

                mapped_file_ptr =	mapped_vec_shared->at(std::distance(file_name_list.begin(), iter_files));

                try {
                    if(mapped_file_ptr == NULL) {
                        throw file_system_excep::offset_exception("[** Mapped pointer is null. **]");
                    }

                    struct stat *file_status =  file_offset_object.get_file_status();

                    mapped_file_ptr->size =	file_status->st_size;

                    mapped_file_ptr->file = file_offset_object.get_popen_file();

                    if(mapped_file_ptr->size == 0 || mapped_file_ptr->file == -1) {
                        throw file_system_excep::offset_exception("[** File size don't get status **]");
                    }


                    mapped_file_ptr->data = (uint8_t *)mmap(0,
                            mapped_file_ptr->size,
                            PROT_READ,
                            MAP_PRIVATE,
                            mapped_file_ptr->file,
                            0);

                    logger->write_info("Mapped file ptr size  ", boost::lexical_cast<std::string>(mapped_file_ptr->size));
                    logger->write_info("Mapped file ptr name  ", boost::lexical_cast<std::string>(mapped_file_ptr->file));
										logger->write_info("Mapped file data      ", boost::lexical_cast<std::string>(mapped_file_ptr->data));

                    if(mapped_file_ptr->data == MAP_FAILED) {
                        throw file_system_excep::offset_exception("[** File cannot map **]");
                        file_offset_object.close_file();
                    }

                } catch(file_system_excep::offset_exception& offset_excep) {
                    logger->write_info("Error, Mapped file", offset_excep.message_error_file_is_null());
                    continue;

                }
            }
        }

        return true; //mapped_vec_shared;
    }

    template<typename FileType, typename MAPPED_FILE>
    std::vector<MAPPED_FILE *> & file_offset_handler<FileType, MAPPED_FILE>
    ::get_mapped_file()
    {
        return *mapped_vec_shared.get();
    }

    template<typename FileType, typename MAPPED_FILE>
    bool  file_offset_handler<FileType, MAPPED_FILE>::unmapped_file(std::vector<MAPPED_FILE * > mapped_vec)
    {
        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_files;

        for(iter_mapped_files = mapped_vec.begin();
                iter_mapped_files != mapped_vec.end();
                ++iter_mapped_files) {
            MAPPED_FILE *mapped_file_ptr = *iter_mapped_files;
            munmap(mapped_file_ptr->data, mapped_file_ptr->size);
            close(mapped_file_ptr->file);
        }

    }


    template class file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>;
    //    template class file_offset_handler<struct common_openfile_type, struct MAPPED_FILE_PE>;

}




