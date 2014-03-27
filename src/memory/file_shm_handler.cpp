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

/*  Titles			                                                     Authors	          Date
 * - File mapping memory supported multithead controller             R.Chatsiri         23/03/2014
 */

#include "memory/file_mem_handler.hpp"

namespace memory
{
    /*Phase-1 :  File read to string on vector-shm */
    template<typename MAPPED_FILE>
    bool file_shm_handler<FileAddress>::initial_shm(uint64_t full_file_size,
            std::map<std::string, MAPPED_FILE *> files_map)
    {
        uint64_t  summary_file_size;

        files_buff_map::const_iterator iter_files;

        for(iter_files = files_map.begin(); iter_files != files_map.end(); iter_files++) {
            MAPPED_FILE   *mf =  *iter_files;

            //encode with MD5 with mf->file_name ( filename inculded path of file)
            for(int count_data = 0; count_data < mf->size(); count_data++) {
                mf->data[count_data];
            }
        }

        //md5_str plan to instanst get_process_id_name
        //boostinp::managed_shared_memory 
        file_shm = new boost::inp::boostinp::managed_shared_memory(create_only, 
					get_process_id_name(), 
					summary_file_size);

        char_allocator  char_alloc(shm.get_segment_manager());
        string_allocator str_alloc(shm.get_segment_manager());

        bs_shm_vec = shm.contrcut<binary_string_shm_vec>(file_name)(str_alloc);
        bs_shm_vec//

    }

    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::insert_file(std::vector<uint8_t> binary_vec,
            uint64_t     file_size,
            const char *file_name)
    {


        bs_shm_vec = shm.contrcut<binary_string_shm_vec>(file_name)(str_alloc);


    }

    /*Phase-2 : File mapped set file load to shared memory

    template<typename MAPPED_FILE>
    bool  file_shm_handler<MAPPED_FILE>::create_mapped()
    {


    }

    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::set_mapped_file(const char *file_name)
    {


    }

    template<typename MAPPED_FILE>
    fdesc_shm_str  file_shm_handler<MAPPED_FILE>::file_desc()
    {


    } */


}

