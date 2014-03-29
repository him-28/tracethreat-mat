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
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace memory
{
		namespace boostinp = boost::interprocess;
	  namespace boostbmi = boost::multi_index;

		typedef boostinp::managed_shared_memory::allocator<char>::type  char_allocator;
    typedef boostinp::basic_string<char, std::char_traits<char>, char_allocator> shm_string;

    struct file_shm_meta{

        uint64_t     file_name_md5;
        uint8_t      status;
        void        *mapped_addr;
        uint64_t     mapped_size;
        shm_string    name;
        file_shm_meta( uint64_t file_name_md5_,
                uint8_t  status_,
                void    *mapped_addr_,
                uint64_t mapped_size_,
                shm_string  name_,
                const char_allocator& ca)
            : file_name_md5(file_name_md5_),
              status(status_),
              mapped_addr(mapped_addr_),
              mapped_size(mapped_size_),name(name_, ca)
        { }
    };


    //Tags
    struct file_name_md5 {};
    struct status {};
    struct mapped_addr {};
    struct mapped_size {};
    struct name {};

    template<typename MAPPED_FILE>
    class file_shm_base
    {

        public:
            typedef boost::shared_ptr<struct file_shm_meta> fshm_meta_str;
            virtual fshm_meta_str get_file_shm_meta(uint64_t file_name_md5)  = 0;

    };


}



#endif /* MEMORY_FILE_SHM_BASE_HPP */

