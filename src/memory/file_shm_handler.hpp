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
 * - Add managed_shared_ptr supported file-map-shm        R.chatsiri       28/03/2014
 * - Phase-2 supported file-mapped with multithread       R.Chatsiri       -
 */

//external
//#include <boost/interprocesses/detail/config_begin.hpp>
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
    namespace h_util = hnmav_util;

    template<typename MAPPED_FILE>
    class file_shm_handler /* : public file_shm_base<MAPPED_FILE> */
    {

        public:
            /*Phase-1 : File read to string on vector-shm */

            //typedef boostinp::managed_weak_ptr<>
            /*
            struct shared_shm_ptr_owner {
            shared_shm_ptr_owner(const shared_shm_ptr_owner& other_shared_shm_ptr)
            : shared_shm_ptr_type(other_shared_shm_ptr)
            {}

            shared_shm_ptr_owner(const shared_shm_ptr_owner& other_owner)
            : shared_ptr(other_owner)
            {}

            //public:
            shared_shm_ptr_type shared_ptr_;

            };
            */

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

            typedef boostinp::map<key_types,
                    binary_string_shm,
                    std::less<key_types>,
                    map_str_shm_allocator> map_str_shm;

            //map allocator
            /*
             //vector string allocator
            //typedef boostinp::vector<binary_string_shm,
            //        string_allocator> binary_string_shm_vec;

            //vector string allocator
            //typedef boostinp::allocator<binary_string_shm_vec,
            //        boostinp::managed_shared_memory::segment_manager> binary_string_shm_vec_allocator;

            			// value vector type
            typedef std::pair<const uint64_t, binary_string_shm_vec> value_types; // vector<char> is value

            			// map vector allocator
            typedef boostinp::allocator<value_types,
            								boostinp::managed_shared_memory::segment_manager> map_shm_allocator;

            typedef boostinp::map<key_types,
                    binary_string_shm_vec,
                    std::less<key_types>,
                    map_shm_allocator> map_shm;
            			*/



            //typedef std::map<std::string, MAPPED_FILE *> files_buff_map;

            //struct type_to_share { } ; : map_shm used for shared type.
            //shared_ptr supported shm-intial
            // typedef boostinp::managed_shared_ptr<map_shm,
            //        boostinp::managed_shared_memory> map_shm_shared_ptr;

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

            bool delete_file_shm();

            std::vector<uint64_t>  get_file_name_md5();

            bool status_file_shm();

            uint64_t size_file_shm();


            bool alignment_file_shm();

            //map_str_shm  get_map_str_shm();
            //file_shm_handler<MAPPED_FILE>::map_str_shm get_map_str_shm();
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

            //virtual fshm_meta_str get_shm_file_meta(uint64_t file_name_md5){ }

            /*Phase-2 : File mapped set file load to shared memory
                       *
                       typename boost::shared_ptr<MAPPED_FILE> mapped_file;
                       typename boost::shared_ptr<struct file_desc_shm> fdesc_shm_str;

                       virtual fdesc_shm_str file_desc();
                       bool  create_mapped();
                       bool  set_mapped_file(const char * file_name);
                       */

        private:
            /*Phase-1 File read to string on vector-shm */
            //shared_shm_ptr_owner *shm_owner;

            boostinp::managed_shared_memory *file_shm;
            //map: key is MD5, value is vecter<char> type.
            //map_shm *map_shm_ptr;
            //map , key : md5, value: string
            map_str_shm *map_str_shm_ptr;
            //vector contains addresses of files sizes.
            std::vector<char *>  addr_df_hex_vec;

            std::vector<uint64_t>  file_name_md5_vec;
            //support multiple allocator
            boostinp::managed_shared_memory::multiallocation_chain *multi_alloca_chain;
            //managed_shared_memory::size_type * sizes; : Not supported.
						
						std::map<const uint64_t, size_t>  map_file_size;

						//logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

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

//#include <boost/interprocesses/detail/config_end.hpp>

#endif
