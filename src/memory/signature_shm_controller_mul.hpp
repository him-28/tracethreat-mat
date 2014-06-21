#ifndef MEMORY_SIGNATURE_SHM_CONTROLLER_HPP
#define MEMORY_SIGNATURE_SHM_CONTROLLER_HPP
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
 * -Factory class initials shm-signature type    R.Chatsiri         20/03/2014
 * -New concept create abstract call signature_shm_controller before filter with drive class.
 *  -----------------------------------------------
 *  vector for insert std::vector<struct meta_sig*>;
 *  struct for shm : meta_sig_mem
 *  signature_shm *  sig_shm = new signature_shm_pe_controller();
 *  sig_shm->initial_shm_sigtype(std::vector<SignatureTypeMemory> * sig_type);
 *  sig_shm->get_signature();                                       04/06/2014
 */
//#include <boost/interprocess/detail/config_begin.hpp>
//#include <boost/interprocess/detail/workaround.hpp>

//#include <boost/interprocess/managed_shared_memory.hpp>
//#include <boost/interprocess/allocators/allocator.hpp>
//#include <boost/interprocess/containers/string.hpp>

//#include <boost/multi_index_container.hpp>
//#include <boost/multi_index/indexed_by.hpp>
//#include <boost/multi_index/ordered_index.hpp>
//#include <boost/multi_index/member.hpp>
//#include <boost/multi_index/tag.hpp>

#include <boost/lexical_cast.hpp>

#include "utils/logger/clutil_logger.hpp"

#include "memory/signature_shm_base.hpp"

namespace memory
{
    namespace boostinp = boost::interprocess;
    namespace bmi  = boost::multi_index;
    namespace h_util = hnmav_util;



    template<typename SignatureTypeMemory, typename SignatureInternal>
    class signature_shm_controller : public signature_shm<SignatureTypeMemory, SignatureInternal>
    {

        public:

						signature_shm_controller();
						
            typedef bmi::multi_index_container<
            meta_sig_mem,
            bmi::indexed_by<
            bmi::ordered_unique<
            bmi::tag<sig>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, sig)
            >,
            bmi::ordered_non_unique<
            bmi::tag<vir_name>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, vir_name)
            >            
            >,
            boostinp::managed_shared_memory::allocator<meta_sig_mem>::type
            > signature_set;
					/*
	            typedef bmi::multi_index_container<
            meta_sig_mem,
            bmi::indexed_by<
            bmi::ordered_unique<
            bmi::tag<sig>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, sig)
            >,
            bmi::ordered_non_unique<
            bmi::tag<vir_name>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, vir_name)
            >,
            bmi::ordered_non_unique<
            bmi::tag<sig_detail>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, sig_detail)
            >,
            bmi::ordered_non_unique<
            bmi::tag<sig_type>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, uint8_t, sig_type)
            >,
            bmi::ordered_non_unique<
            bmi::tag<offset>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, uint64_t, offset)
            >
            >,
            boostinp::managed_shared_memory::allocator<meta_sig_mem>::type
            > signature_set;
						*/

            typedef std::vector<SignatureTypeMemory *>  sigtype_mem_vec_type;
            typedef boost::interprocess::managed_shared_memory  managed_shm_type;

            //Initial all signature send to Signature-SHM
            bool initial_shm_sigtype(std::vector<SignatureTypeMemory *> *sigtype,
                    std::string shm_name);

            virtual bool filter_sigtype(boost::interprocess::managed_shared_memory *managed_shm,
                    std::string shm_name) = 0;

            virtual std::vector<SignatureTypeMemory *>& get_signature() = 0;

            virtual SignatureTypeMemory  verify_signature(const uint8_t sig_type,
                    const uint64_t start_index,
                    const uint64_t end_index){

						}
						
						~signature_shm_controller();

        protected:
            sigtype_mem_vec_type *sigtype_mem_vec_ptr;
            managed_shm_type      *managed_shm_ptr;
            signature_set    *sig_set_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

    template<typename SignatureTypeMemory, typename SignatureInternal>
    class signature_shm_pe_controller :
        public signature_shm_controller<SignatureTypeMemory, SignatureInternal>
    {
        public:

					 typedef bmi::multi_index_container<
            meta_sig_mem,
            bmi::indexed_by<
            bmi::ordered_unique<
            bmi::tag<sig>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, sig)
            >,
            bmi::ordered_non_unique<
            bmi::tag<vir_name>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, vir_name)
            >            
            >,
            boostinp::managed_shared_memory::allocator<meta_sig_mem>::type
            > signature_set;
						/*
            typedef bmi::multi_index_container<
            meta_sig_mem,
            bmi::indexed_by<
            bmi::ordered_unique<
            bmi::tag<sig>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, sig)
            >,
            bmi::ordered_non_unique<
            bmi::tag<vir_name>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, vir_name)
            >,
            bmi::ordered_non_unique<
            bmi::tag<sig_detail>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, shm_string, sig_detail)
            >,
            bmi::ordered_non_unique<
            bmi::tag<sig_type>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, uint8_t, sig_type)
            >,
            bmi::ordered_non_unique<
            bmi::tag<offset>,
            BOOST_MULTI_INDEX_MEMBER(meta_sig_mem, uint64_t, offset)
            >
            >,
            boostinp::managed_shared_memory::allocator<meta_sig_mem>::type
            > signature_set;
						*/


            typedef boost::interprocess::managed_shared_memory  managed_shm_type;

						signature_shm_pe_controller();

            ~signature_shm_pe_controller();

            //Filter use only PE signature for scanning.
            bool filter_sigtype(boost::interprocess::managed_shared_memory *managed_shm,
                    std::string shm_name);
            //SHM for PE Signature.
            virtual SignatureTypeMemory verify_signature(const uint8_t sig_type,
                    const uint64_t start_index,
                    const uint64_t end_index) { }

            virtual std::vector<SignatureTypeMemory *>& get_signature() { }
        private:
            //managed_shm_type   *sig_shm_pe;
            managed_shm_type   *managed_shm_ptr;
						signature_set    *sig_set_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}





#endif /* MEMORY_SIGNATURE_SHM_CONTROLLER_HPP */
