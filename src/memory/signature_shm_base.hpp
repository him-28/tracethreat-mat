#ifndef MEMORY_SIGNATURE_SHM_BASE_HPP
#define MEMORY_SIGNATURE_SHM_BASE_HPP
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
/*  Titles			                                 Authors	          Date
 *-Signature base structure pass data          R.Chatsiri     20/03/0214
 */

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

//[doc_multi_index
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
//
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/unordered_map.hpp>
#include <functional>
#include <boost/functional/hash.hpp>

namespace memory
{

    namespace boostinp = boost::interprocess;
    namespace bmi  = boost::multi_index;
    namespace h_util = hnmav_util;


    typedef  boost::interprocess::managed_shared_memory::allocator<char>::type   char_allocator;
    typedef  boost::interprocess::basic_string<char, std::char_traits<char>, char_allocator>  shm_string;

    struct meta_sig {
        uint8_t  sig_type; // MD5, SHA-1, SHA-256
        uint64_t offset; // offset start search virus
        const char *sig;   // signature
        const char *vir_name;  // virus name
        const char *sig_detail;  // Declare detail of virus
			  meta_sig(): 
					sig_type(0), 
					offset(0), 
					sig(""), 
					vir_name(""), 
					sig_detail(""){ }

    };


    struct meta_sig_mem {

        shm_string sig;
        shm_string vir_name;
        shm_string sig_detail;

        uint8_t sig_type; // MD5, SHA-1, SHA-256
        uint64_t offset;  // start offset scannint.

        meta_sig_mem(
                const char *_sig,
                const char *_vir_name,
                const char *_sig_detail,
                uint8_t _sig_type,
                uint64_t _offset,
                const char_allocator& a):
            sig(_sig, a),
            vir_name(_vir_name, a),
            sig_detail(_sig_detail, a),
            sig_type(_sig_type),
            offset(_offset) { }
    };


    struct sig {};
    struct vir_name {};
    struct sig_detail {};
    struct sig_type {};
    struct offset {};

    typedef std::pair<shm_string, meta_sig_mem> msig_mem_type;

    typedef boost::interprocess::
    allocator<msig_mem_type, boost::interprocess::managed_shared_memory::segment_manager>
    shm_msig_mem_allocator;

    typedef boost::unordered_map<shm_string
    ,meta_sig_mem
    ,boost::hash<shm_string>
    ,std::equal_to<shm_string>
    ,shm_msig_mem_allocator>  msig_mem_map;


    template<typename SignatureTypeMemory, typename SignatureInternal>
    class signature_shm
    {

        public:
            //signature_shm();

            virtual std::vector<SignatureTypeMemory *>& get_signature() = 0;

            virtual bool initial_shm_sigtype(std::vector<SignatureTypeMemory *>   *sigtype,
                    std::string shm_name) = 0;

            //support TBB scannig
            virtual SignatureTypeMemory  verify_signature(const uint8_t sig_type,
                    const uint64_t start_index,
                    const char start_symbol,
                    const uint64_t end_index,
                    const char end_symbol,
                    const char *detail_symbol) = 0;
            //support OCL multiple length find in binary_vec.
            virtual SignatureTypeMemory  verify_signature(const uint8_t sig_type,
                    const char start_symbol,
									  uint64_t   start_pos_symbol,
                    std::vector<char> *binary_vec,
                    std::vector<char> *symbol_vec) = 0;

            //~signature_shm();
    };


}

#endif /* MEMORY_SIGNATURE_SHM_BASE_HPP */
