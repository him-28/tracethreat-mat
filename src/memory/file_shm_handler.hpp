#ifndef MEMORY_FILE_MEM_HANDLER__HPP
#define MEMORY_FILE_MEM_HANDLER__HPP

/*
* Copyright 2014 Chatsiri Rattana.
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
 * - Add managed_shared_ptr supported file-map-shm        R.chatsiri       28/03/2014
 * - Phase-2 supported file-mapped with multithread       R.Chatsiri       -
 * - Phase-1 
 *   		 - UUID for create name of SHM.
 *       - MAPPED_FILE_PE set flag internal and external scanning.
 *                                                       R.Chatsiri       28/04/2014
 */

//external
#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include "utils/get_process_id_name.hpp"

#include "utils/logger/clutil_logger.hpp"


//internal
#include "memory/file_shm_base.hpp"

namespace memory
{

    namespace boostinp = boost::interprocess;

    template<typename MAPPED_FILE>
    class file_shm_handler
    {

        public:
           
						file_shm_handler();


            //char allocator
            typedef boostinp::allocator<char,
                    boostinp::managed_shared_memory::segment_manager> char_allocator;

            //create string from char allocator
            typedef boostinp::basic_string<char,
                    std::char_traits<char>, char_allocator> binary_string_shm;

            //string allocator
            typedef boostinp::allocator<binary_string_shm,
                    boostinp::managed_shared_memory::segment_manager> string_allocator;

            //boost::interprocess::map.
            typedef uint64_t key_types; // MD5 is key

            // value string type
            typedef std::pair<const uint64_t, binary_string_shm>  value_types_str;

            // map string allocator
            typedef boostinp::allocator<value_types_str,
                    boostinp::managed_shared_memory::segment_manager> map_str_shm_allocator;

						// map interprocess.
            typedef boostinp::map<key_types,
                    binary_string_shm,
                    std::less<key_types>,
                    map_str_shm_allocator> map_str_shm;

           
            typedef boost::shared_ptr<struct file_shm_meta> fshm_meta_str;

            /**
            * @brief Initial SHM and declared size of memory on it's.
            *
            * @param full_file_size  all size of files calculate to full_file_size.
            *
            * @return True, intial memory supported file-shm completed.
            */
            bool initial_shm(uint64_t full_file_size);

            /**
            * @brief Initial File get binary of it's send to vector string-SHM.
            *
            * @param files_map std::vector<MAPPED_FILE *> files_map
            *
            * @return True, initial success or false not success.
            */
            bool initial_file_shm(std::vector<MAPPED_FILE *> * files_map);


            // get list char on file-shm
            /**
            * @brief Insert index of map for get hex char on map
            *
            * @param index  uint64_t is index of map.
            *
            * @return True, get index success. False have not index on file.
            */
            bool list_detail_shm(const uint64_t *file_name_md5);

						bool set_shm_name(std::string uuid);

						const std::string get_shm_name();

            bool delete_file_shm();

            std::vector<uint64_t>  get_file_name_md5();

            typename file_shm_handler<MAPPED_FILE>::map_str_shm& get_map_str_shm();

            /**
            * @brief Get map file_name_md5 and size of file.
            *
            * @return return map contain data in order to check filename and size of files.
            */
            std::map<const uint64_t, size_t> *get_map_file_size();

            ~file_shm_handler() {
                //file_shm->destroy_ptr(map_str_shm_ptr);
            }

            
        private:
            /*Phase-1 File read to string on vector-shm */
            boostinp::managed_shared_memory *file_shm;
            //map , key : md5, value: string
            map_str_shm *map_str_shm_ptr;
            //vector contains addresses of files sizes.
            std::vector<char *>  addr_df_hex_vec;

            std::vector<uint64_t>  file_name_md5_vec;
            //support multiple allocator
            boostinp::managed_shared_memory::multiallocation_chain *multi_alloca_chain;
						
						std::map<const uint64_t, size_t>  map_file_size;

						std::string shm_name;

						//logger
            boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
            utils::clutil_logging<std::string, int>    *logger;

           
        protected:

    };

}

#endif
