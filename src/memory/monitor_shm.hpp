#ifndef MEMORY_MONITOR_SHM_HPP
#define MEMORY_MONITOR_SHM_HPP

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
 * -Monitor SHM on system                                         R.Chatsiri       04/04/2014
 */

namespace memory
{

    struct ShmDetail {
        int64_t file_name_md5;
        int64_t binary_size;
    };


    template<typename ShmDetail>
    class imemory
    {
        public:
            virtual std::vector<shm::struct_index> get_shm_vec(uint64_t md5_index) = 0;
            virtual bool processes_inital(std::vector<struct meta_shm>   *meta_shm_vec) = 0;

    };

    template<typename ShmDetail>
    class memory_controller
    {

        public:

            virtual std::vector<shm::struct_index> get_shm_vec(uint64_t md5_index);
            virtual bool processes_inital(std::vector<struct meta_shm>   *meta_shm_vec);

        protected:
            virtual const char *list_data(const char *shm_name);
            virtual const char *size_data(const char *shm_name);
            virtual const char *delete_data(const char *shm_name, const char *index);

            virtual const char *add_data(const char *shm_key,
                    const char *shm_value,
                    uint64_t index_vec);

            virtual uint64_t     get_blank_element();

    };

    template<typename ShmDetail>
    class monitor_shm : memory_controller<ShmDetail>
    {
        public:


        private:

    };



}




#endif /* MEMORY_MONITOR_SHM_HPP */
