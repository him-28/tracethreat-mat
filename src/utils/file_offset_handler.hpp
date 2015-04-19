#ifndef UTILS_FILE_OFFSET_HANDLER__HPP
#define UTILS_FILE_OFFSET_HANDLER__HPP

/*
* Copyright 2014 R.Chatsiri, Inc.
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
 *- File_offset_handler gets offset from file mapped.  R.Chatsiri    05/01/2014
 *- Create file_map support thread controller          R.Chatsiri    23/03/2014
 * - Changed variable received value from mapped_file_vec_shared to shared_ptr
 *   contains struct of data in vector name vector<MAPPED_FILE*> . 
 *                                                     R.Chatsiri    05/03/2014
 */

//#define FILE_DESCRIPTOR  int

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <list>
#include "sys/mman.h"

//#include "filetypes/pe.hpp"
#include "filetypes/pe_template.hpp"
#include "utils/file_handler.hpp"
#include "exception/system_exception.hpp"

//
#include "utils/logger/clutil_logger.hpp"

namespace utils
{

    namespace file_system_excep = trace_exception::file_system;

    template<typename  MAPPED_FILE = struct MAPPED_FILE_PE,
             typename MEMORY_BLOCK = struct MEMORY_BLOCK_PE>
    class file_offset_detail
    {
        public:
            std::vector<MAPPED_FILE *>& get_mappedfile_vec() {
                return mapped_file_vec_ptr;
            }

            std::vector<MEMORY_BLOCK *>& get_memory_block_vec() {
                return memory_block_vec_ptr;
            }
        private:
            std::vector<MAPPED_FILE *> mapped_file_vec_ptr;
            std::vector<MEMORY_BLOCK *> memory_block_vec_ptr;
    };

    template<typename FileType = struct common_filetype>
    class pack_file_offset
    {
        public:
            std::vector<uint8_t> get_offset_pack() const;

        private:
            std::vector<uint8_t> offset;
            std::string  file_name;
    };

    template<typename FileType = struct common_filetype, typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class file_offset_handler : public file_handler<FileType>, public pack_file_offset<FileType>
    {
        public:
            typedef std::vector<MAPPED_FILE *> map_file_vec;
            typedef boost::shared_ptr<map_file_vec> mapped_vec_sptr;
						typedef mapped_vec_sptr  mapped_vec_ptr;
            file_offset_handler();
            /**
            * @brief File pointer points file name in order to scanning.
            *
            * @param p_file pointer from file_handler(base class)
            *
            * @return True, if able get all offset.
            */
            bool set_file_offset(typename FileType::file_ptr *p_file);

            /**
            * @brief Seek file from file-mapped.
            *
            * @return File packs offset from seek file.
            */
            std::vector<pack_file_offset<FileType> >  offset_perfile()const;

            /**
            * @brief
            *
            * @param file_name input file name or directory file.
            * @param mapped_vec structure MAPPED_FILE type container.
            *
            * @return True, success init file mapped.
            */
            bool  mapped_file(std::vector<const char*> file_name,
                    std::vector<MAPPED_FILE *> mapped_vec,
                    file_offset_handler<FileType, MAPPED_FILE>& file_offset_object,
										const char * file_sig);

						bool  mapped_file(std::vector<const char*> * file_name,
                    std::vector<MAPPED_FILE *> * mapped_vec,
                    file_offset_handler<FileType, MAPPED_FILE>& file_offset_object);


            /**
            * @brief Ummap single file from memory.
            *
            * @param mapped_vec  Struct MAPPED_FILE types include details
            * for file mapped on memory area.
            *
            * @return True, Success for file unmapped on memory.
            */
            bool unmapped_file(std::vector<MAPPED_FILE * > mapped_vec);

 
            /**
            * @brief Pass by value. Member function returns shared_ptr constain vector
            *
            * @return boost::shared_ptr<std::vector<MAPPED_FILE * > > It's contain data mapped file.
            */
            mapped_vec_ptr get_mappedf_vec_ptr();

						~file_offset_handler();

        private:

            //ifile<common_filetype> *ifh;

            mapped_vec_ptr mapped_vec_shared;

            //MAPPED_FILE *mapped_file_ptr;

            //logger
            boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
            utils::clutil_logging<std::string, int>    *logger;

    };



}



#endif
