#ifndef POLICY_SCAN_FILE_CONTROLLER_HPP
#define POLICY_SCAN_FILE_CONTROLLER_HPP

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
 * -Register class to class_register                      R.Chatsiri       12/09/2014
 */

#include <boost/shared_ptr.hpp>

#include "utils/base/common.hpp"

#include "filetypes/pe_file_controller.hpp"

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"

#include "threadconcurrency/tbbpostscan_pe_controller.hpp"

#include "scan/file_scan_policy.hpp"

#include "msg/message_tracethreat.pb.h"

//logger
#include "utils/logger/clutil_logger.hpp"

namespace policy
{
    using namespace memory;
    using namespace utils;

    namespace fpolicy = policy;
    namespace h_util = hnmav_util;

    template<typename MAPPED_FILE>
    class scan_internet_controller
    {

        public:
						typedef message_tracethreat::InfectedFileInfo  threatinfo_type;
					
						typedef std::vector<threatinfo_type*> threatinfo_vec_type;

            virtual bool load_database(std::vector<struct utils::meta_sig *> *meta_sig_vec,
                    std::string shm_sig_name) = 0;

            virtual bool load_engine(utils::filetype_code file_type) = 0;

            virtual  bool find_engine(utils::filetype_code  file_type) = 0;

            virtual  threatinfo_vec_type & scan_file() = 0;

            virtual  bool set_file(std::vector<MAPPED_FILE *>   *mapped_file_vec,
                    std::vector<const char *>     *file_type_vec) = 0;

    };

    //Input file to scanning.
    template<typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class scan_pe_internet_controller : public scan_internet_controller<MAPPED_FILE>
    {

        public:

            typedef  tbbscan::iactire_engine<char, tbbscan::tbb_allocator> iactire_concur_engine_type;

						typedef message_tracethreat::InfectedFileInfo  threatinfo_type;

						typedef std::vector<threatinfo_type*> threatinfo_vec_type;

            virtual bool load_database(std::vector<struct utils::meta_sig *> *meta_sig_vec,
                    std::string shm_sig_name);

            virtual bool load_engine(utils::filetype_code file_type);

            virtual bool find_engine(utils::filetype_code  file_type);

            virtual threatinfo_vec_type & scan_file();

            virtual bool set_file(std::vector<MAPPED_FILE *>   *mapped_file_vec,
                    std::vector<const char *>     *file_type_vec);

            std::vector<utils::file_scan_result<MAPPED_FILE> *> get_scan_result();

            std::string get_name_controller()const;

            ~scan_pe_internet_controller();

        private:

            std::string shm_sig_name;

            std::vector<struct utils::meta_sig *> *meta_sig_vec;


            memory::file_shm_handler<MAPPED_FILE>  f_shm_handler;

            memory::signature_shm_pe_controller <struct memory::meta_sig, struct memory::meta_sig_mem>
                        sig_shm_pe;

            tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator> sig_engine;

            iactire_concur_engine_type *iactire_concur_engine_scanner;

            std::string sigtype_code;

						threatinfo_vec_type threatinfo_vec;

            //Scan step
            fpolicy::file_scan_policy<struct MAPPED_FILE_PE> *pef_policy;

            fpolicy::scan_file_policy<
            MAPPED_FILE_PE,
            fpolicy::pe_policy_is<fpolicy::pe_file_policy<MAPPED_FILE_PE>, MAPPED_FILE_PE>
            > sf_policy;

            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;


    }; // scan_internet_controller


}// policy

#endif /* POLICY_SCAN_FILE_CONTROLLER_HPP */
