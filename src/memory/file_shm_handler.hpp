#ifndef MEMORY_FILE_MEM_HANDLER__HPP
#define MEMORY_FILE_MEM_HANDLER__HPP

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
 * - File memory controllers interprocesses between scanning and memory buffer
 *                                                        R.Chatsiri       07/02/2014
 * - Register class for initial memory for file buffer    R.Chatsiri       07/02/2014
 * - Phase-1 supported vector and string SHM.             R.Chatsiri       25/03/2014
 *   Multithrad using file-mapped (mmap) loads offset of files.
 *                                                        R.Chatsiri       25/03/2014
 * - Phase-2 supported file-mapped with multithread       R.Chatsiri       -
 */

//external
#include <boost/interprocesses/detail/config_begin.hpp>
#include <boost/interprocesses/file_mapping.hpp>
#include <boost/interprocesses/mapped_region.hpp>

//internal
#include "memory/file_shm_base.hpp"

namespace memory
{
    namespace boostinp = boost::interprocesses;
    template<typename MAPPED_FILE>
    class file_shm_handler : public file_shm_base<MAPPED_FILE>
    {

        public:
            /*Phase-1 : File read to string on vector-shm */
            struct type_to_share { } ;
            //shared_ptr supported shm-intial
            typedef boostinp::managed_shared_ptr<type_to_share,
                    managed_shared_memory> shared_shm_ptr_type;
            //typedef boostinp::managed_weak_ptr<>
            struct shared_shm_ptr_owner {
                shared_shm_ptr_owner(const shared_shm_ptr_owner& other_shared_shm_ptr)
                    : shared_shm_ptr_type(other_shared_shm_ptr);
                {}

                shared_shm_ptr_ower(const shared_shm_ptr_ower& other_owner)
                    : shared_ptr(other_owner)
                {}

                //public:
                shared_shm_ptr_type shared_ptr_;

            };
            typedef boostinp::allocator<char,
                    boostinp::managed_shared_memory::segment_manager> char_allocator;

            typedef boostinp::basic_string<char,
                    std::char_traits<char>, char_allocator> binary_string_shm;

            typedef boostinp::allocator<binary_string_shm,
                    boostinp::managed_shared_memory::segment_manager> string_allocator;

            typedef boostinp::vector<binary_string_shm,
                    string_allocator> binay_string_shm_vec;
			
						typedef std::map<std::string, MAPPED_FILE*> files_buff_map;

            /**
            * @brief Initial File get binary of it's send to vector string-SHM.
            *
            * @param full_file_size  size of file in bytes.
						* @param files_map  std::string contains MD5 of file., MAPPED_FILE is file type and file
						* name.
						*
            * @return True, initial success or false not success.
            */
            bool initial_shm(uint64_t full_file_size, std::map<std::string, MAPPED_FILE*> files_map);

            /**
            * @brief Insert file for multithread got file send to OCL.
            *
            * @param binary_vec File binary type.
            * @param file_size  File size of binary.(bytes)
            * @param file_name  File name.
            *
            * @return
            */
            bool insert_file(std::vector<uint8_t> binary_vec, uint64_t file_size, const char *file_name);

            /*Phase-2 : File mapped set file load to shared memory
            *
            typename boost::shared_ptr<MAPPED_FILE> mapped_file;
            typename boost::shared_ptr<struct file_desc_shm> fdesc_shm_str;

            virtual fdesc_shm_str file_desc();
            bool  create_mapped();
            bool  set_mapped_file(const char * file_name);
           */
				protected:

        private:
            /*Phase-1 File read to string on vector-shm */
            shared_shm_ptr_owner * shm_owner;
            binary_string_shm_vec *bs_shm_vec;
						boostinp::managed_shared_memory * file_shm;	
            /*Phase-2 : File mapped set file load to shared memory
            *
            mapped_file  mapped_file_sptr;

            boost::interprocesses::file_mapping  * fmapping;
            boost::interprocesses::mapped_region * mapping_reg;

            const char  * file_name;
            const size_t  file_size;
            */

        protected:
            /*Phase-2 :
            //TODO: Change type of vector to allocated with internal allocated did not standard.
            virtual std::vector<char> write_to_memory();

            fdesc_shm_str fdes_shm_str_sptr;
            */


    };

}

#endif
