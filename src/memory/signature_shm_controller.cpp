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
 * - Boost initial malware/virus/rootkit factory of shm
 *                                               R.Chatsiri         06/06/2014
 * - Fix problem from binary_buffer[symbol_vec->size()] = '\0'; Why array size increase.
 *                                               R.Chatsiri         09/06/2014
 */


#include <algorithm>
#include <utility>

#include "memory/signature_shm_controller.hpp"

namespace memory
{

    template<typename SignatureTypeMemory, typename SignatureInternal>
    signature_shm_controller<SignatureTypeMemory, SignatureInternal>::
    signature_shm_controller()
    {


    }

    //Initial size of SIG-SHM per kinds such PE,ELF and PROCESS
    template<typename SignatureTypeMemory, typename SignatureInternal>
    bool signature_shm_controller<SignatureTypeMemory, SignatureInternal>::
    initial_shm_sigtype(std::vector<SignatureTypeMemory *> *sigtype,
            std::string shm_name)
    {

        if(sigtype->empty()) {
            logger->write_info("signature_shm_controller::initial_shm_sigtype, vector-sig size is zero");
            return false;
        }

        typename std::vector<SignatureTypeMemory *>::iterator iter_sig_vec;

        for(iter_sig_vec = sigtype->begin();
                iter_sig_vec != sigtype->end();
                ++iter_sig_vec) {

            //std::vector<SignatureTypeMemory*> all types of malicious signature.
            //filter type of typing sush only PE,ELF and Process.
            if(filter_sigtype(*iter_sig_vec, shm_name)) {
                logger->write_info("signature_shm_controller::initial_shm_sigtype, filter type Success",
                        shm_name);
                //success for filter type.
            }


        }



        return false;
    }

    template<typename SignatureTypeMemory, typename SignatureInternal>
    signature_shm_controller<SignatureTypeMemory, SignatureInternal>::
    ~signature_shm_controller()
    {
        //managed_shm_ptr->destroy_ptr(sig_set_ptr);
    }

    //-------------------------- signature_shm_pe_controller --------------------------

    template<typename SignatureTypeMemory, typename SignatureInternal>
    signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    signature_shm_pe_controller()
    {

    }

    template<typename SignatureTypeMemory, typename SignatureInternal>
    bool signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    filter_sigtype(SignatureTypeMemory   *sig_mem,
            std::string shm_name)
    {
        if(sig_mem->sig_type == utils::pe_file) {
            // insert structure to map.
            logger->write_info("signature_shm_pe_controller::filter_sigtype, sig_name",
                    boost::lexical_cast<std::string>(sig_mem->sig));

            sig_map.insert(std::make_pair(*sig_mem->sig, sig_mem));
						sig_vec.push_back(sig_mem);
        }

        logger->write_info("signature_shm_pe_controller::filter_sigtype, Start filtering process");
        logger->write_info("signature_shm_pe_controller::filter_sigtype, map size ",
                boost::lexical_cast<std::string>(sig_map.size()));
        return true;
    }

    template<typename SignatureTypeMemory, typename SignatureInternal>
    SignatureTypeMemory signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    verify_signature(const uint8_t sig_type,
            const char start_symbol,
            uint64_t   start_pos_symbol,
            std::vector<char> *binary_vec,
            std::vector<char> *symbol_vec)
    {

        logger->write_info("signature_shm_pe_controller::filter_sigtype, map size ",
                boost::lexical_cast<std::string>(sig_map.size()));

        if(symbol_vec->empty()) {
            logger->write_info("signature_shm_pe_controller::filter_sigtype, symbol is zero size");
            signature_ret_ptr = new SignatureTypeMemory();
            return *signature_ret_ptr;
        }

        typename sigtype_map::iterator iter_map;
        typename std::vector<uint64_t>::iterator iter_sigsize_def_vec;

        uint64_t symbol_size = symbol_vec->size();

        //logger->write_info_test("signature_shm_pe_controller::filter_sigtype, symbol_size",
        //        boost::lexical_cast<std::string>(symbol_size));

        char *symbol_buffer = new char[symbol_vec->size()];
        char *binary_buffer = new char[symbol_vec->size()];

        std::copy(symbol_vec->begin(), symbol_vec->end(), symbol_buffer);

        //first steps mapping. Initial support signature encode with MD5, SHA-1, SHA-256
        //Check size MD5, SHA-1 : 32 bits
        //Check size SHA256     : 64 bits
        if(std::find(this->sigsize_def_vec.begin(),
                this->sigsize_def_vec.end(),
                symbol_size) != this->sigsize_def_vec.end()) {

            //logger->write_info_test("signature_shm_pe_controller::filter_sigtype, sig support lengths",
            //        boost::lexical_cast<std::string>(symbol_size));

            //copy and search on map.
            std::copy(binary_vec->begin() + start_pos_symbol,
                    binary_vec->begin() + start_pos_symbol + symbol_vec->size(),
                    binary_buffer);

            typename sigtype_map::const_iterator sig_ret =  sig_map.find(*binary_buffer);

            if(sig_ret != sig_map.end()) {

                signature_ret_ptr = sig_ret->second;

								logger->write_info("------------------Result Scanning---------------------------");				

                logger->write_info("signature_shm_pe_controller::filter_sigtype, found virus sig",
                        boost::lexical_cast<std::string>(signature_ret_ptr->virname));
                logger->write_info("signature_shm_pe_controller::filter_sigtype, virus signature",
                        boost::lexical_cast<std::string>(signature_ret_ptr->sig));

                logger->write_info("------------------Scanning Completed.-----------------------");

                return *signature_ret_ptr;
            }


            logger->write_info("signature_shm_pe_controller::filter_sigtype, not found");

        } else {
            int64_t symbol_pos_end =  start_pos_symbol + symbol_vec->size();


            // copy binary to binary buffer. Copy use size of binary as symbol_vec.
            // binary_vec[........140.........150......]
            // binary_buffer[104..........150]
            std::copy(binary_vec->begin() + start_pos_symbol,
                    binary_vec->begin() + start_pos_symbol + symbol_vec->size(),
                    binary_buffer);

						
            //logger->write_info_test("signature_shm_pe_controller::filter_sigtype, symbol_buffer size",
            //        boost::lexical_cast<std::string>(strlen(symbol_buffer)));

            //logger->write_info_test("signature_shm_pe_controller::filter_sigtype, binary_buffer size",
            //        boost::lexical_cast<std::string>(strlen(binary_buffer)));

            //fix binary equal symbol_buffer: problems from std::copy renew size.
            //Safety compare.
            binary_buffer[symbol_vec->size()] = '\0';
						symbol_buffer[symbol_vec->size()] = '\0';
            logger->write_info_test("signature_shm_pe_controller::filter_sigtype, symbol sig",
                    boost::lexical_cast<std::string>(symbol_buffer));

            logger->write_info_test("signature_shm_pe_controller::filter_sigtype, Infected binary",
                    boost::lexical_cast<std::string>(binary_buffer));

            //if(strncmp(symbol_buffer, binary_buffer, symbol_vec->size()) < 0) {
            if(std::strcmp(symbol_buffer, binary_buffer) != 0){
                logger->write_info("Signature incorrect size. Internal incorrect.");
                signature_ret_ptr = new SignatureTypeMemory();
                return *signature_ret_ptr;
            }

            //Search on the fly
            typename sigtype_map::const_iterator sig_ret =  sig_map.find(*binary_buffer);
            typename sigtype_map::iterator iter_sig;

						/*
            for(iter_sig = sig_map.begin();
                    iter_sig != sig_map.end();
                    ++iter_sig) {
                std::pair<char, SignatureTypeMemory *> pair_sig = *iter_sig;
                logger->write_info("signature_shm_pe_controller::filter_sigtype, sig on map",
                        boost::lexical_cast<std::string>(pair_sig.first));
            }
						*/

            if(sig_ret != sig_map.end()) {
                signature_ret_ptr = sig_ret->second;
                logger->write_info_test("signature_shm_pe_controller::filter_sigtype, found virus sig",
                        boost::lexical_cast<std::string>(signature_ret_ptr->virname));
                logger->write_info_test("signature_shm_pe_controller::filter_sigtype, virus signature",
                        boost::lexical_cast<std::string>(signature_ret_ptr->sig));
                logger->write_info("signature_shm_pe_controller::filter_sigtype, Completed processes");

                return *signature_ret_ptr;
            }

            logger->write_info("signature_shm_pe_controller::filter_sigtype, not found");
        }

        signature_ret_ptr = new SignatureTypeMemory();
        logger->write_info("signature_shm_pe_controller::filter_sigtype,Not found, Completed processes");
        return *signature_ret_ptr;

    }

    template<typename SignatureTypeMemory, typename SignatureInternal>
    SignatureTypeMemory signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    verify_signature(const uint8_t sig_type,
            const uint64_t start_index,
            const char start_symbol,
            const uint64_t end_index,
            const char end_symbol,
            const char *detail_symbol)
    {


    }

    template<typename SignatureTypeMemory, typename SignatureInternal>
    signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    ~signature_shm_pe_controller()
    {
        //managed_shm_ptr->destroy_ptr(sig_set_ptr);
    }

    template<typename SignatureTypeMemory, typename SignatureInternal>
    std::vector<SignatureTypeMemory *>& signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    get_signature()
    {
				return sig_vec;
    }

    template class signature_shm_controller<struct utils::meta_sig, struct meta_sig_mem>;

    template class signature_shm_pe_controller<struct utils::meta_sig, struct meta_sig_mem>;


}

//#include <boost/interprocess/detail/config_end.hpp>

