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
 *  -Result Infected Container. Propose for send to RocksDB.         R.Chatsiri   09/06/2014
 */
#include "memory/signature_shm_base.hpp"
#include "utils/connector/result_infected.hpp"
#include "utils/base/system_code.hpp"
#include <vector>
#include <string>
#include <string.h>

#define VIRUS_SIG 2
class ResultInfectedTest : public ::testing::Test
{

    protected:

        virtual void SetUp() {
            //load binary file pe.
            struct memory::meta_sig *msig;
            msig = &meta_sig_def[0];
            msig->sig_type = utils::pe_file;
            msig->offset   = 140;
            msig->sig      = "e9e1bb0812ab2344b1b1e9db09\0";
            msig->vir_name = "Trojan-Case-01\0";
            msig->sig_detail = "Trojan for testing-01.\0";
            meta_sig_vec.push_back(&meta_sig_def[0]);

            msig = &meta_sig_def[1];
            msig->sig_type = utils::pe_file;
            msig->offset   = 2556;
            msig->sig      = "a8e2bb0812ab2344b1b1e9db09\0";
            msig->vir_name = "Trojan-Case-02\0";
            msig->sig_detail = "Trojan for testing-02.\0";
            meta_sig_vec.push_back(&meta_sig_def[1]);


            sig_type = utils::pe_file;
            start_symbol = 0;
            start_pos_symbol = 4;
        }
        //support sig_shm_pe.verify_signature() OCL.
        uint8_t sig_type;
        char    start_symbol;
        uint64_t start_pos_symbol;
        std::vector<char> binary_vec;
        std::vector<char> symbol_vec;
        //support intial filter
        struct memory::meta_sig meta_sig_def[VIRUS_SIG];
        std::vector<struct memory::meta_sig *>  meta_sig_vec;

};


TEST_F(ResultInfectedTest, result_infected)
{


}
