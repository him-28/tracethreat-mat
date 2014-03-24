#ifndef MEMORY_FILE_SHM_BASE_HPP
#define MEMORY_FILE_SHM_BASE_HPP

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
 *  -File shm base for mmap file.                                    R.Chatsiri         21/03/2014
 */

namespace memory
{

    typename basic_string<char, std::char_traits<char>, char_allocator> shm_string;

    struct file_desc_shm {

        uint64_t      id;
        uint8_t      status;
        void        *mapped_addr;
        uint64_t     mapped_size;
        shm_string    name;
        file_desc_shm( uint64_t id_,
                uint8_t  status_,
                void    *mapped_addr_,
                uint64_t mapped_size_,
                shm_string  name_,
                const char_allocator& ca)
            : id(id_), status(status_), mapped_addr(mapped_addr_), mapped_size(mapped_size_),name(name_, ca)
        { }
    };


    //Tags
    struct id{};
		struct status{};
		struct mapped_addr{};
		struct mapped_size{};
		struct name{};

            template<typename MAPPED_FILE>
class file_shm_base
    {

        public:
            typename boost::shared_ptr<struct file_desc_shm> fdesc_shm_str;
            virtual fdesc_shm_str file_desc()  = 0;

    };


}



#endif /* MEMORY_FILE_SHM_BASE_HPP */

