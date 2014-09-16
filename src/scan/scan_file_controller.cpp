/*
* Copyright 2014 Chatsiri Rattana.
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

/*  Titles																								Authors					 Date
 *- Register class support scan_file_controller           R.Chatsiri
 */

#include "scan/scan_file_controller.hpp"

namespace policy
{

    //Initial signature and shm-name for signature shared_memory.
    //Plan-00004: Initial SHM Signature type.
    template<typename MAPPED_FILE>
    bool scan_pe_internet_controller<MAPPED_FILE>::
    load_database(std::vector<struct utils::meta_sig *>   *meta_sig_vec,
            std::string shm_sig_name)
    {
        sig_shm_pe.initial_shm_sigtype(meta_sig_vec, shm_sig_name);
    }

    //Load engine.-
    template<typename MAPPED_FILE>
    bool scan_pe_internet_controller<MAPPED_FILE>::load_engine(utils::filetype_code  file_type)
    {
			   //Signature Engine initials for scanning.
        if(!sig_engine.create_engine(this->meta_sig_vec, file_type)) {
            logger->write_info("Engine-PE cannot intial for internet scanning.");
						return false;
        }
     
        //Register Engine for scanning.
        sigtype_code = utils::filetype_code_map(file_type);
        //Create Register Class
        tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::register_actire_type(sigtype_code,
                tbbscan::actire_pe_engine<char, tbbscan::tbb_allocator>::create);
        //Create ACTire engine from signature that register
        iactire_concur_engine_scanner = tbbscan::actire_engine_factory<char, tbbscan::tbb_allocator>::
                create_actire_engine(sigtype_code);
				return true;
    }



    //Initial file received from client
    template<typename MAPPED_FILE>
    bool scan_pe_internet_controller<MAPPED_FILE>::scan_file()
    {
        if(pef_policy != NULL) {
            logger->write_info("PE-File Policy is NULL, Scan_pe_internet cannot scan virus");
            return false;
        }

        sf_policy.scan_pe(pef_policy,&sig_shm_pe, &sig_engine, iactire_concur_engine_scanner);

        return true;
    }

    template<typename MAPPED_FILE>
    bool scan_pe_internet_controller<MAPPED_FILE>::set_file(std::vector<MAPPED_FILE *>   *mapped_file_vec)
    {
        //send to scan_pe policy based class.
        pef_policy = new fpolicy::pe_file_policy<struct MAPPED_FILE_PE>();
        pef_policy->set_mapped_file(mapped_file_vec);
    }

    template<typename MAPPED_FILE>
    std::vector<utils::file_scan_result<MAPPED_FILE> *> 
			scan_pe_internet_controller<MAPPED_FILE>::get_scan_result()
    {

    }

    template<typename MAPPED_FILE>
    std::string scan_pe_internet_controller<MAPPED_FILE>::get_name_controller() const
    {
				return "Name controller is : scan_pe_internet_controller";
    }

}
