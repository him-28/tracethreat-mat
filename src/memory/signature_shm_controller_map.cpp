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
 */

//#include <boost/interprocess/detail/config_begin.hpp>
//#include <boost/interprocess/detail/workaround.hpp>



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
				/*

        if(sigtype->empty()) {
            logger->write_info("signature_shm_controller::initial_shm_sigtype, vector-sig size is zero");
            return false;
        }

        sigtype_mem_vec_ptr = sigtype;
        //calculate size of struct
        uint64_t shm_size = sizeof(SignatureTypeMemory) *sigtype->size();
        //name shm type.
        //deleted shm
        boostinp::shared_memory_object::remove(shm_name.c_str());
        //create shm
        managed_shm_ptr = new boostinp::managed_shared_memory(boostinp::create_only,
                shm_name.c_str(),
                shm_size);

        logger->write_info("signature_shm_controller::initial_shm_sigtype, shm-size",
                boost::lexical_cast<std::string>(shm_size));


        //std::vector<SignatureTypeMemory*> all types of malicious signature.
        //filter type of typing sush only PE,ELF and Process.
        if(filter_sigtype(managed_shm_ptr, shm_name)) {
            logger->write_info("signature_shm_controller::initial_shm_sigtype, filter type Success",
                    shm_name);
            //success for filter type.
            return true;
        }


        //check size correct.
        if(managed_shm_ptr->get_size() != shm_size) {
            logger->write_info("signature_shm_controller::initial_shm_sigtype, Size not equal of SHM name",
                    boost::lexical_cast<std::string>(shm_name));
            return false;
        }


        logger->write_info("signature_shm_controller::initial_shm_sigtype, filter type NOT Success",
                shm_name);
				*/
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
    bool signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    initial_shm_sigtype(std::vector<SignatureTypeMemory *> *sigtype,
            std::string shm_name)
    {
        if(sigtype->empty()) {
            logger->write_info("signature_shm_controller::initial_shm_sigtype, vector-sig size is zero");
            return false;
        }

        sigtype_mem_vec_ptr = sigtype;
        //calculate size of struct
        uint64_t shm_size = sizeof(SignatureTypeMemory) *sigtype->size();
        //name shm type.
        //deleted shm
        boostinp::shared_memory_object::remove(shm_name.c_str());
        //create shm
        boostinp::managed_shared_memory shm(boostinp::create_only,
                shm_name.c_str(),
                shm_size);

        logger->write_info("signature_shm_controller::initial_shm_sigtype, shm-size",
                boost::lexical_cast<std::string>(shm_size));


		    memory::msig_mem_map *sig_map =
                shm.construct<msig_mem_map>("shm_internal_pe")
                (3, boost::hash<memory::shm_string>(), std::equal_to<memory::shm_string>(),
                        shm.get_allocator<memory::msig_mem_type>());

			/*
        //std::vector<SignatureTypeMemory*> all types of malicious signature.
        //filter type of typing sush only PE,ELF and Process.
        if(filter_sigtype(managed_shm_ptr, shm_name)) {
            logger->write_info("signature_shm_controller::initial_shm_sigtype, filter type Success",
                    shm_name);
            //success for filter type.
            return true;
        }
			*/

        //check size correct.
        if(managed_shm_ptr->get_size() != shm_size) {
            logger->write_info("signature_shm_controller::initial_shm_sigtype, Size not equal of SHM name",
                    boost::lexical_cast<std::string>(shm_name));
            return false;
        }


        logger->write_info("signature_shm_controller::initial_shm_sigtype, filter type NOT Success",
                shm_name);

        return false;

    }


    template<typename SignatureTypeMemory, typename SignatureInternal>
    signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    signature_shm_pe_controller()
    {

    }
    template<typename SignatureTypeMemory, typename SignatureInternal>
    bool signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    filter_sigtype(boost::interprocess::managed_shared_memory *managed_shm,
            std::string shm_name)
    {

        logger->write_info("signature_shm_pe_controller::filter_sigtype, Start filtering process");

        typename std::vector<SignatureTypeMemory *>::iterator iter_sigtype_vec;

        memory::msig_mem_map *sig_map =
                managed_shm->construct<msig_mem_map>("shm_internal_pe")
                (3, boost::hash<memory::shm_string>(), std::equal_to<memory::shm_string>(),
                        managed_shm->get_allocator<memory::msig_mem_type>());

        char_allocator ca(managed_shm->get_allocator<char>());

        for(iter_sigtype_vec = this->sigtype_mem_vec_ptr->begin();
                iter_sigtype_vec != this->sigtype_mem_vec_ptr->end();
                ++iter_sigtype_vec) {
            SignatureTypeMemory *sig_mem_exter = *iter_sigtype_vec;
            //Plan-00004 : validate fields of struct external
            //-----------------------------------------------
            //Convert External struct meta_sig(Contain details of sig )  to meta_sig_mem(internal-SHM)
            SignatureInternal sig_internal(sig_mem_exter->sig,
                    sig_mem_exter->vir_name,
                    sig_mem_exter->sig_detail,
                    sig_mem_exter->sig_type,
                    sig_mem_exter->offset,
                    ca);

            sig_map->insert(msig_mem_type(shm_string(sig_mem_exter->sig, ca), sig_internal));

            logger->write_info("signature_shm_pe_controller::filter_sigtype, Sig-SHM-PE, sig_name",
                    boost::lexical_cast<std::string>(sig_mem_exter->sig));
            logger->write_info("signature_shm_pe_controller::filter_sigtype, Sig-SHM-PE, vir_name",
                    boost::lexical_cast<std::string>(sig_mem_exter->vir_name));

        }

        logger->write_info("signature_shm_pe_controller::filter_sigtype, Start filtering process");
        return true;
    }

    template<typename SignatureTypeMemory, typename SignatureInternal>
    signature_shm_pe_controller<SignatureTypeMemory, SignatureInternal>::
    ~signature_shm_pe_controller()
    {
        //managed_shm_ptr->destroy_ptr(sig_set_ptr);
    }


    template class signature_shm<struct meta_sig, struct meta_sig_mem>;

    template class signature_shm_controller<struct meta_sig, struct meta_sig_mem>;

    template class signature_shm_pe_controller<struct meta_sig, struct meta_sig_mem>;


}

//#include <boost/interprocess/detail/config_end.hpp>

