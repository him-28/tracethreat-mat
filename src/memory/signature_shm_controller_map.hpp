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

#include <boost/lexical_cast.hpp>

#include "utils/logger/clutil_logger.hpp"

#include "memory/signature_shm_base.hpp"

namespace memory
{

    template<typename SignatureTypeMemory, typename SignatureInternal>
    class signature_shm_controller : public signature_shm<SignatureTypeMemory, SignatureInternal>
    {

        public:

            signature_shm_controller();


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
                    const uint64_t end_index) {

            }

            ~signature_shm_controller();

        protected:
            sigtype_mem_vec_type *sigtype_mem_vec_ptr;
            managed_shm_type      *managed_shm_ptr;
            //signature_set    *sig_set_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

    template<typename SignatureTypeMemory, typename SignatureInternal>
    class signature_shm_pe_controller 
        //:   public signature_shm_controller<SignatureTypeMemory, SignatureInternal>
    {
        public:
            typedef std::vector<SignatureTypeMemory *>  sigtype_mem_vec_type;
            typedef boost::interprocess::managed_shared_memory  managed_shm_type;

            signature_shm_pe_controller();

            ~signature_shm_pe_controller();


            bool initial_shm_sigtype(std::vector<SignatureTypeMemory *> *sigtype,
                    std::string shm_name);

            //Filter use only PE signature for scanning.
            bool filter_sigtype(boost::interprocess::managed_shared_memory *managed_shm,
                    std::string shm_name);
            //SHM for PE Signature.
            virtual SignatureTypeMemory verify_signature(const uint8_t sig_type,
                    const uint64_t start_index,
                    const uint64_t end_index) { }

            virtual std::vector<SignatureTypeMemory *>& get_signature() { }
        private:
            sigtype_mem_vec_type *sigtype_mem_vec_ptr;

            managed_shm_type   *managed_shm_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}





#endif /* MEMORY_SIGNATURE_SHM_CONTROLLER_HPP */
