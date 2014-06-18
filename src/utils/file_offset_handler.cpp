/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                          Authors	         Date
 *
 */

#include <iostream>

#include "file_offset_handler.hpp"

namespace utils
{


    template<typename FileType, typename MAPPED_FILE>
    file_offset_handler<FileType, MAPPED_FILE>
    ::file_offset_handler()
    {

        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        //logger->write_info_test("Init logger fileoffset");
    }


    template<typename FileType,  typename MAPPED_FILE>
    bool file_offset_handler<FileType, MAPPED_FILE>
    ::set_file_offset(typename FileType::file_ptr *p_file)
    {


    }

    template<typename FileType,  typename MAPPED_FILE>
    std::vector<pack_file_offset<FileType> >
    file_offset_handler<FileType, MAPPED_FILE>
    ::offset_perfile()const
    {


    }


    template<typename FileType, typename MAPPED_FILE>
    bool file_offset_handler<FileType, MAPPED_FILE>
    ::mapped_file(std::vector<const char*> file_name_vec,
            std::vector<MAPPED_FILE *> mapped_vec,
            file_offset_handler<FileType, MAPPED_FILE>& file_offset_object,
						const char * file_sig)
    {
        std::vector<const char*>::iterator iter_files;

        mapped_vec_shared = boost::make_shared<std::vector<MAPPED_FILE * > >();
				
        const char *file_name;
        std::string s_file_name;
				std::string s_file_sig = file_sig;
        MAPPED_FILE *mapped_file_ptr;

        if(!file_name_vec.size() || !mapped_vec.size()) {
            logger->write_info("Not data on file_name or mapped file");
            return false;
        }

        for(iter_files = file_name_vec.begin();
                iter_files != file_name_vec.end();
                ++iter_files) {

            try {
                s_file_name = *iter_files;

                if(s_file_name.empty())
                    throw file_system_excep::offset_exception("[** File is null **]");


                logger->write_info("File path for mmaped ", s_file_name);

                //Set file name to mapped structure

                file_name  = s_file_name.c_str();

            } catch(file_system_excep::offset_exception& offset_excep) {
                logger->write_info("Error, file offset",
                        offset_excep.message_error_file_is_null());
                continue;
            }

            if(file_offset_object.set_filepath(file_name)) {

                if(!file_offset_object.file_read_mapped()) {
                    throw file_system_excep
                    ::offset_exception("[** File cannot open path **]");
                }
								
								mapped_file_ptr = mapped_vec.at(std::distance(file_name_vec.begin(), iter_files));
                try {
                    if(mapped_file_ptr == NULL) {
                        throw file_system_excep
                        ::offset_exception("[** Mapped pointer is null. **]");
                    }

                    struct stat *file_status =  file_offset_object.get_file_status();

                    mapped_file_ptr->size =	file_status->st_size;

                    mapped_file_ptr->file = file_offset_object.get_popen_file();
										/*
                    logger->write_info("Mapped file ptr size  ",
                            boost::lexical_cast<std::string>(mapped_file_ptr->size));

                    logger->write_info("Mapped file ptr name  ",
                            boost::lexical_cast<std::string>(mapped_file_ptr->file));
										*/
                    if(mapped_file_ptr->size == 0 || mapped_file_ptr->file == -1) {
                        throw file_system_excep::offset_exception("[** File size don't get status **]");
                    }
				
								mapped_file_ptr->file_name += s_file_name;
								mapped_file_ptr->file_sig  += std::string(file_sig);// s_file_sig;

								std::cout<<"file_offset_handler::mapped_file " << mapped_file_ptr->file_name <<std::endl;

										
                    mapped_file_ptr->data = (uint8_t *)mmap(0,
                            mapped_file_ptr->size,
                            PROT_READ,
                            MAP_SHARED,// MAP_PRIVED, it's not changed/shared.
                            mapped_file_ptr->file,
                            0);
										
                    //logger->write_info("Mapped file with mmap success");

                    
                    //logger->write_info("Mapped file name completed ", s_file_name);

                    //logger->write_info("Mapped file data ",
                    //        boost::lexical_cast<std::string>(mapped_file_ptr->data));

										mapped_vec_shared->push_back(mapped_file_ptr);
										
                    if(mapped_file_ptr->data == MAP_FAILED) {
                        throw file_system_excep
                        ::offset_exception("[** File cannot map **]");
                        file_offset_object.close_file();
                    }
										
                } catch(file_system_excep::offset_exception& offset_excep) {
                    logger->write_info("Error, Mapped file",
                            offset_excep.message_error_file_is_null());
                    continue;

                }
            }
        }
			  file_name_vec.clear(); // clear file name on list

        return true;
    }

    template<typename FileType, typename MAPPED_FILE>
    typename file_offset_handler<FileType, MAPPED_FILE>::mapped_vec_ptr 
			file_offset_handler<FileType, MAPPED_FILE>
    ::get_mappedf_vec_ptr()
    {
        return mapped_vec_shared;
    }



    template<typename FileType, typename MAPPED_FILE>
    bool  file_offset_handler<FileType, MAPPED_FILE>
    ::unmapped_file(std::vector<MAPPED_FILE * > mapped_vec)
    {
        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_files;

        for(iter_mapped_files = mapped_vec.begin();
                iter_mapped_files != mapped_vec.end();
                ++iter_mapped_files) {
            MAPPED_FILE *mapped_file_ptr = *iter_mapped_files;
						/*TODO: Cannot print why ?
						logger->write_info(" Ummaped file completed.", 
								boost::lexical_cast<std::string>(mapped_file_ptr->file_name));
						*/
            munmap(mapped_file_ptr->data, mapped_file_ptr->size);
            close(mapped_file_ptr->file);

		        }
			return true;
    }

	  template<typename FileType, typename MAPPED_FILE>
    file_offset_handler<FileType, MAPPED_FILE>::~file_offset_handler(){

		}

    template class file_offset_handler<struct common_filetype,
             struct MAPPED_FILE_PE>;

}




