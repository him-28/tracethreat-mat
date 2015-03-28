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

#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>

#include "utils/logger/clutil_logger.hpp"

#include "memory/signature_shm_base.hpp"

#include "utils/base/system_code.hpp"

namespace memory
{

    template<typename SignatureTypeMemory, typename SignatureInternal>
    class signature_shm_controller : public signature_shm<SignatureTypeMemory, SignatureInternal>
    {

        public:

            signature_shm_controller();


            typedef std::vector<SignatureTypeMemory *>  sigtype_mem_vec_type;

            //Initial all signature send to Signature-SHM
            /**
            * @brief Initiail SHM per signature type. Meta_sig add to Share Memory. Plan-00003 support only
            * std::map, but in Plan-00004 implement Full Shared-Memory.
            * - Work around use vector contain meta_sig for multithread.
            * @param sigtype Vector contains signature all.
            * @param shm_name classify signature for each type such PE, ELF and Process.
            *
            * @return Success return true.
            */
            bool initial_shm_sigtype(std::vector<SignatureTypeMemory *> *sigtype,
                    std::string shm_name);

            //for interitance to pe,elf and process types.
            virtual bool filter_sigtype(SignatureTypeMemory *sig_mem,
                    std::string shm_name) = 0;

            std::vector<SignatureTypeMemory *>& get_signature() { };
            //support TBB scannig
            SignatureTypeMemory  verify_signature(const uint8_t sig_type,
                    const uint64_t start_index,
                    const char start_symbol,
                    const uint64_t end_index,
                    const char end_symbol,
                    const char *detail_symbol) { };

            //support OCL multiple length find in binary_vec.
            SignatureTypeMemory  verify_signature(const uint8_t sig_type,
                    const char start_symbol,
                    uint64_t   start_pos_symbol,
                    std::vector<char> *binary_vec,
                    std::vector<char> *symbol_vec) { };


            ~signature_shm_controller();

        protected:
            //signature vector
            sigtype_mem_vec_type *sigtype_mem_vec_ptr;

            std::vector<uint64_t> sigsize_def_vec;  // MD5, SHA-1, SHA-256

            boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
            utils::clutil_logging<std::string, int>    *logger;

    };

    template<typename SignatureTypeMemory, typename SignatureInternal>
    class signature_shm_pe_controller :
        public signature_shm_controller<SignatureTypeMemory, SignatureInternal>
    {
        public:
            typedef boost::unordered_map<char, SignatureTypeMemory *> sigtype_map;

            signature_shm_pe_controller();

            ~signature_shm_pe_controller();

            //Filter use only PE signature for scanning.
            bool filter_sigtype(SignatureTypeMemory *sig_mem,
                    std::string shm_name);

            //support TBB scannig
            SignatureTypeMemory  verify_signature(const uint8_t sig_type,
                    const uint64_t start_index,
                    const char start_symbol,
                    const uint64_t end_index,
                    const char end_symbol,
                    const char *detail_symbol);
            //support OCL multiple length find in binary_vec.
            SignatureTypeMemory  verify_signature(const uint8_t sig_type,
                    const char start_symbol,
                    uint64_t   start_pos_symbol,
                    std::vector<char> *binary_vec,
                    std::vector<char> *symbol_vec);




            std::vector<SignatureTypeMemory *>& get_signature();//{ return sig_vec; };
        private:

            //signature unordered map
            sigtype_map  sig_map;
            SignatureTypeMemory *signature_ret_ptr;
            //signature in vector.
            std::vector<SignatureTypeMemory *>  sig_vec;

            //logger
            boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
            utils::clutil_logging<std::string, int>    *logger;

    };


}





#endif /* MEMORY_SIGNATURE_SHM_CONTROLLER_HPP */
