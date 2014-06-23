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
 *  -Init Signature SHM test with malare sigature.                   R.Chatsiri   20/03/2014
 *  -First signature insert to PE-SHM signature kind.                R.Chatsiri   06/06/2014
 */

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"
#include "utils/base/system_code.hpp"
#include <vector>
#include <string>
#include <string.h>

#define VIRUS_SIG 2
class SignatureShmTest : public ::testing::Test
{

    protected:



        virtual void SetUp() {
            //load binary file pe.
            struct utils::meta_sig *msig;
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

            char *data_sig1 = "2332a8e2bb0812ab2344b1b1e9db09a123b01232343\0";
            char *end_data_sig1 = data_sig1 + strlen(data_sig1);
            binary_vec.insert(binary_vec.end(), data_sig1, end_data_sig1);

            //char *data_sig2 = "a132308e2bb0812ab2344b1b1e9db0923a1ab";
            //char *end_data_sig2 = data_sig2 + strlen(data_sig2);

            char *symbol_db = "a8e2bb0812ab2344b1b1e9db09\0";
            char *end_symbol_db = symbol_db + strlen(symbol_db);
            symbol_vec.insert(symbol_vec.end(), symbol_db, end_symbol_db);

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
        struct utils::meta_sig meta_sig_def[VIRUS_SIG];
        std::vector<struct utils::meta_sig *>  meta_sig_vec;
        memory::signature_shm_pe_controller<struct utils::meta_sig, struct memory::meta_sig_mem>
                    sig_shm_pe;

};


TEST_F(SignatureShmTest, signature_shm_pe_controller)
{
    std::string shm_name  = "shm-pe";

    sig_shm_pe.initial_shm_sigtype(&meta_sig_vec, shm_name);
    struct utils::meta_sig msig_ret = 
				sig_shm_pe.verify_signature(sig_type, start_symbol, start_pos_symbol, &binary_vec, &symbol_vec);
	  EXPECT_EQ("a8e2bb0812ab2344b1b1e9db09", std::string(msig_ret.sig));
		
    //typename std::vector<<struct utils::meta_sig *>::const_iterator iter_sig_vec;
    /*
    for(iter_sig_vec = meta_sig_vec.begin();
    		iter_sig_vec != meta_sig_vec.end();
    		++iter_sig_vec)
    {
    struct utils::meta_sig msg  = meta_sig_vec
    sig_shm_pe.verify_signature(msg->sig_type,"0",1,&binary_vec, &symbol_vec);
    }
    */
}

TEST_F(SignatureShmTest, verify_signature)
{

    //    sig_shm_pe.verify_signature(sig_type, start_symbol, &binary_vec, &symbol_vec);

}
