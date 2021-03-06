#ifndef INTERNET_SECURITY_AES_CONTROLLER_HPP
#define INTERNET_SECURITY_AES_CONTROLLER_HPP

#define KEY_SIZE 16
#define KEY_ENCRYPTION_SIZE 128
#define MAX_PADDING_LEN 16

#define MAX_MAP_AES_SIZE 1024
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
#include <folly/AtomicHashMap.h>

#include <vector>
#include <iostream>
#include <memory>

#include "internet/msg/packedmessage_scan.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "utils/base/system_code.hpp"

namespace internet
{

    namespace security
    {

        //class internet::security::aes_controller<message_scan::ResponseScan>;
        template<typename MessageType, typename EncryptType> class aes_controller;
        template<typename EncryptType> class encryption_controller;
        class encryption_type;
        struct aes_cbc;

				//enum crypto_type{ INSERT, FIND};

        struct aes_cbc {
            char *ip;
            char *uuid;

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


            aes_cbc(std::string& _ip, std::string& _uuid):
                input_length(0),
                output_length(0),
                enc_length(0),
                dec_length(0),
                key_length(0) {
                ip = new char[_ip.size()+1];
                std::copy(_ip.begin(), _ip.end(), ip);
                ip[_ip.size()] = '\0';

                uuid = new char[_uuid.size()+1];
                std::copy(_uuid.begin(), _uuid.end(), uuid);
                uuid[_uuid.size()] = '\0';

                LOG(INFO)<<" Struct IP : "<< ip <<", UUID : "<< uuid;

            }

            aes_cbc(const char   *_ip, const char *_uuid) :
                input_length(0),
                output_length(0),
                enc_length(0),
                dec_length(0),
                key_length(0) {


                const std::size_t size_len_ip = strlen(_ip);
                ip = new char[size_len_ip+1];
                strncpy(ip, _ip, size_len_ip);
                ip[size_len_ip] = '\0';

                const std::size_t size_len_uuid = strlen(_uuid);
                uuid = new char[size_len_uuid+1];
                strncpy(uuid, _uuid, size_len_uuid);
                uuid[size_len_uuid] = '\0';

                LOG(INFO)<<" Struct IP : "<< ip <<", UUID : "<< uuid;

            }


            aes_cbc(std::string _ip,
                    std::string _uuid,
                    unsigned char *_key,
                    unsigned char *_iv):
                input_length(0),
                output_length(0),
                enc_length(0),
                dec_length(0),
                key_length(0) {

                ip = new char[_ip.size()+1];
                std::copy(_ip.begin(), _ip.end(), ip);
                ip[_ip.size()] = '\0';

                uuid = new char[_uuid.size()+1];
                std::copy(_uuid.begin(), _uuid.end(), uuid);
                uuid[_uuid.size()] = '\0';

                //memcpy(key, _key, KEY_SIZE+1);
								std::copy(_key, _key+KEY_SIZE+1, key);
								key[KEY_SIZE] = '\0'; 
                
                std::copy(_iv, _iv + AES_BLOCK_SIZE+1, iv);
                iv[AES_BLOCK_SIZE] = '\0';

                //memcpy(iv, _iv, AES_BLOCK_SIZE+1);

                LOG(INFO)<<" Struct IP : "<< ip <<", UUID : "<< uuid;
								LOG(INFO)<<"Key : " << key;
                LOG(INFO)<<"IV  : " << iv;
            }



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


                if(ip_comp && uuid_comp) {
                    return true;
                }

                return false;
            }
        };


        template<typename EncryptType = aes_cbc>
        class encryption_controller
        {

            public:
                encryption_controller() { }

                virtual bool find_key(const char *ip, const char *uuid) = 0;

                virtual EncryptType *filter_key(const char *ip, const char *uuid) = 0;

                virtual bool encryption_msgs(const char *msg, int msg_length, EncryptType   *enc_type) =  0;

                virtual bool decryption_msgs(const char *msg, int msg_length, EncryptType   *enc_type) =  0;

                virtual EncryptType& get_crypto()const = 0;

                virtual EncryptType *initial_key(std::string ip, std::string uuid) = 0;

								virtual EncryptType * process_crypto(aes_cbc & _aes, utils::crypto_mode  crypto) = 0;

                ~encryption_controller() { }
        };


        template<typename MessageType, typename EncryptType>
        class aes_controller  :
            public encryption_controller<EncryptType>
        {

            public:

                //typedef aes_cbc encrypt_type;
								typedef folly::AtomicHashMap<uint64_t, internet::security::aes_cbc*>  aha_aes_type;

                typedef folly::ConcurrentSkipList<std::unique_ptr<aes_cbc>, UniquePtrComp> skip_list_type;

                typedef skip_list_type::Accessor  skip_list_accessor_type;

                typedef std::vector<uint8_t> msgenc_vec_type;

                aes_controller() : aha_aes(new aha_aes_type(MAX_MAP_AES_SIZE)){ }
               //  : skip_list_ptr(skip_list_type::createInstance()) { }

                bool ping_service();

                bool initial_engine();

                bool find_key(std::string ip, std::string uuid);

                EncryptType *initial_key(std::string ip, std::string uuid);

                bool encryption_msgs(const char *msg, int msg_length, EncryptType *aes);

                bool decryption_msgs(const char *msg, int msg_length, EncryptType *aes);

                bool find_key(const char *ip, const char *uuid);

                EncryptType *filter_key(const char *ip, const char *uuid);

                EncryptType& get_crypto()const {
                    return *aes_ptr;
                }

								EncryptType * process_crypto(aes_cbc & _aes, utils::crypto_mode  crypto);

                /* using for production. Support std::string from protocal-buffer */
                bool encryption_msgs(std::string msg, aes_cbc *aes);

                bool decryption_msgs(std::string msg, aes_cbc *aes);

                void wait_sync();

                void sync();

                ~aes_controller();

                std::unique_ptr<aes_cbc>& get_aes_msgs() const;

            private:
                //ENGINE *engine_encrypts;
                ENGINE *engine_rdrand;
                //aes_cbc *aes;
                //aes_cbc *aes_ptr;
                EncryptType *aes_ptr;
                //std::shared_ptr<skip_list_type> skip_list_ptr = skip_list_type::createInstance();
                std::shared_ptr<skip_list_type> skip_list_ptr;

								std::shared_ptr<aha_aes_type>  aha_aes;

                //skip_list_accessor_type skip_list_accessor;
                AES_KEY dec_key;
                AES_KEY enc_key;
                EVP_CIPHER_CTX ctx_enc;
                EVP_CIPHER_CTX ctx_dec;

                //lock mutex
                bool sync_send;

                typedef boost::mutex mutex_t;
                mutable mutex_t enc_mutex;
                boost::condition_variable cond_;

            		//typename aes_controller<MessageType, EncryptType>::
            		//skip_list_accessor_type skip_list_accessor(skip_list_ptr);
            		//skip_list_type::Accessor skip_list_accessor(skip_list_ptr);

        };


    }

}

#endif /* INTERNET_SECURITY_AES_CONTROLLER_HPP */
