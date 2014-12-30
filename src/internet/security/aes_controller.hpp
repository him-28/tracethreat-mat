#ifndef INTERNET_SECURITY_AES_CONTROLLER_HPP
#define INTERNET_SECURITY_AES_CONTROLLER_HPP

#define KEY_SIZE 16
#define KEY_ENCRYPTION_SIZE 128
#define MAX_PADDING_LEN 16
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

/*  Titles			                                          Authors	         Date
 *- AES (CBC mode) encrypts buffer message communication between server/client.
 *                                                        R.Chatsiri
 */

#include <openssl/engine.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <folly/ConcurrentSkipList.h>

#include <vector>
#include <iostream>
#include <memory>

#include "internet/msg/packedmessage_scan.hpp"


namespace internet
{

    namespace security
    {
        struct aes_cbc {
            char const *ip;
            char const *uuid;

            unsigned char *msg;

						unsigned char *input_msg;
						
            unsigned char *enc_msg;
            int enc_length;
            int input_length;


            unsigned char *dec_msg;
						int dec_length;
						int output_length;


            unsigned char  key[KEY_SIZE];

            unsigned char  iv[AES_BLOCK_SIZE];

            int key_length;


            aes_cbc(std::string& _ip, std::string& _uuid) : ip(_ip.c_str()), 
                  uuid(_uuid.c_str()),
                  input_length(0),
									output_length(0),
                  enc_length(0),
									dec_length(0),
                  key_length(0) { }

            aes_cbc& operator=(const aes_cbc& aes_) {
                if(this != &aes_) {
                    this->ip = aes_.ip;
                    this->uuid = aes_.uuid;
                }//if

                return *this;
            }

            // i//~aes_cbc(){ delete key;

        };

        struct UniquePtrComp {

            bool operator()(const std::unique_ptr<aes_cbc>& aes_x,
                    const std::unique_ptr<aes_cbc>& aes_y)const {

                if(!aes_x) return false;

                if(!aes_y) return false;

                //IP compared
                int ip_comp = aes_x->ip == aes_y->ip;
                //UUID compared
                int uuid_comp = aes_x->uuid == aes_y->uuid;

                if(ip_comp && uuid_comp) return true;

                return false;
            }
        };

        template<typename MessageType>
        class aes_controller
        {

            public:

                typedef folly::ConcurrentSkipList<std::unique_ptr<aes_cbc>, UniquePtrComp> skip_list_type;

                typedef skip_list_type::Accessor  skip_list_accessor_type;

                typedef std::vector<uint8_t> msgenc_vec_type;

                aes_controller() { } // : skip_list_ptr(skip_list_type::createInstance()){ }

                bool ping_service();

                bool initial_engine();

                bool find_key(std::string ip, std::string uuid);

                aes_cbc *initial_key(std::string ip, std::string uuid);

                bool encryption_msgs(std::vector<uint8_t>& msg_vec, aes_cbc *aes);

                bool decryption_msgs(std::vector<uint8_t>& msg_vec, aes_cbc *aes);


                bool encryption_msgs(boost::shared_ptr<MessageType>  msg_vec, aes_cbc *aes);

                bool decryption_msgs(boost::shared_ptr<MessageType>  msg_vec, aes_cbc *aes);


                bool encryption_msgs(const char * msg, std::size_t length, aes_cbc *aes);

                bool decryption_msgs(const char * msg, std::size_t length, aes_cbc *aes);

                bool encryption_msgs(std::string msg, aes_cbc *aes);

                bool decryption_msgs(std::string msg, aes_cbc *aes);




                std::unique_ptr<aes_cbc>& get_aes_msgs() const;

                ~aes_controller() {
                    //ENGINE_finish(engine_rdrand);
                    //ENGINE_free(engine_rdrand);
                    //ENGINE_cleanup();
                }


            private:
                //ENGINE *engine_encrypts;
                ENGINE *engine_rdrand;
                //aes_cbc *aes;
                aes_cbc *aes_ptr;
                std::shared_ptr<skip_list_type> skip_list_ptr = skip_list_type::createInstance();
                //skip_list_accessor_type skip_list_accessor;
                AES_KEY dec_key;
                AES_KEY enc_key;
								EVP_CIPHER_CTX ctx_enc;
								EVP_CIPHER_CTX ctx_dec;

        };


    }

}

#endif /* INTERNET_SECURITY_AES_CONTROLLER_HPP */
