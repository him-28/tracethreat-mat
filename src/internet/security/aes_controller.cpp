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
 * -AES default 128 bit encryption/decryption message buffer
 *                                                        R.Chatsiri
 *
 */
#include <stdio.h>
#include <inttypes.h>

#include "internet/security/aes_controller.hpp"

namespace internet
{

    namespace security
    {

        static void hex_print(const void *pv, size_t len)
        {
            const unsigned char *p = (const unsigned char *)pv;

            if (NULL == pv)
                printf("NULL");
            else {
                size_t i = 0;

                for (; i<len; ++i)
                    printf("%02X ", *p++);
            }

            printf("\n");
        }


        //template<typename MessageType>
        template<typename MessageType, typename EncryptType>
        bool aes_controller<MessageType, EncryptType>::initial_engine()
        {
            //Random engine.

            engine_rdrand = ENGINE_by_id("rdrand");

            if(engine_rdrand == NULL) {
                return false;
            }

            if(!ENGINE_init(engine_rdrand)) {
                return false;
            }

            if(!ENGINE_set_default(engine_rdrand, ENGINE_METHOD_RAND)) {
                return false;
            }

            return true;

        }


        template<typename MessageType, typename EncryptType>
        EncryptType *aes_controller<MessageType, EncryptType>::
        process_crypto(aes_cbc& _aes, utils::crypto_mode crypto)
        {

						LOG(INFO)<<"Process crypto start ...";
						LOG(INFO)<<"AES IP : " << _aes.ip;
						LOG(INFO)<<"AES UUID : "<< _aes.uuid;

            boost::lock_guard<mutex_t> lock_(enc_mutex);

            aes_cbc *aes_ = new aes_cbc(_aes.ip, _aes.uuid, _aes.key, _aes.iv);

            std::stringstream ip_external_stream;
            ip_external_stream << aes_->ip;

            std::stringstream uuid_external_stream;
            uuid_external_stream << aes_->uuid;

            uint64_t uuid_ref = *((uint64_t*)std::string(aes_->uuid).c_str());


            switch(crypto) {

            case utils::insert_key_crypto_mode :
						
					      LOG(INFO)<<"Initial AES key for client IP : "<< aes_->ip;
                LOG(INFO)<<"UUID      : "<< aes_->uuid;
                LOG(INFO)<<"UUID REF  : "<< uuid_ref;


                //Key set.
                memset(aes_->key, 0, KEY_SIZE);

                if(!RAND_bytes(aes_->key, KEY_SIZE)) {
                    LOG(INFO)<<"RAND cannot random KEY";
                }

                //IV set
                memset(aes_->iv, 0, AES_BLOCK_SIZE);

                if(!RAND_bytes(aes_->iv, AES_BLOCK_SIZE)) {
                    LOG(INFO)<<"RAND cannot random IV";
                }

                aes_->key_length = KEY_SIZE;
                  
                //ip_external_stream >> uuid_ref;

                aha_aes->insert(std::make_pair(
                        uuid_ref, new internet::security::aes_cbc(
                                ip_external_stream.str(),
                                uuid_external_stream.str(), aes_->key, aes_->iv))
                              );

                return new aes_cbc(ip_external_stream.str(),
                        uuid_external_stream.str(),
                        aes_->key,
                        aes_->iv);

                break;

            case utils::find_key_crypto_mode :

							  //uuid_external_stream >> uuid_ref;

     
								aes_ = aha_aes->find(uuid_ref)->second;

		            LOG(INFO)<<"Find AES key for client IP : "<< aes_->ip;
                LOG(INFO)<<"Find UUID : "<< aes_->uuid;
					      LOG(INFO)<<"Find UUID : "<< uuid_ref;
								LOG(INFO)<<"Find key : "<< aes_->key;
                LOG(INFO)<<"Find IV : " << aes_->iv;
					

                return aes_;

                break;

            case utils::register_key_crypto_mode:
								
                //uuid_external_stream >> uuid_ref;

					      LOG(INFO)<<"Register AES key for client IP : "<< aes_->ip;
                LOG(INFO)<<"UUID      : "<< aes_->uuid;
                LOG(INFO)<<"UUID Ref  : "<< uuid_ref;
                LOG(INFO)<<"Key  register  : "<< aes_->key;
                LOG(INFO)<<"IV  register   : "<< aes_->iv;

                aha_aes->insert(std::make_pair(
                        uuid_ref, new internet::security::aes_cbc(
                                ip_external_stream.str(),
                                uuid_external_stream.str(), aes_->key, aes_->iv))
                              );

                return &_aes;

            default:

                LOG(INFO)<<"Not found crypto_mode.";

                break;
            };


            return NULL;

        }//process_crypto.

        //template<typename MessageType>
        template<typename MessageType, typename EncryptType>
        EncryptType *aes_controller<MessageType, EncryptType>::
        initial_key(std::string ip, std::string uuid)
        {
            //wait_sync();

            boost::lock_guard<mutex_t> lock_(enc_mutex);

            typename aes_controller<MessageType, EncryptType>::
            skip_list_accessor_type skip_list_accessor(skip_list_ptr);

            std::unique_ptr<aes_cbc> aes_ = folly::make_unique<aes_cbc>(ip, uuid);

            LOG(INFO)<<"Initial AES key for client IP : "<< aes_->ip;
            LOG(INFO)<<"Initial UUID                  : "<< aes_->uuid;

            //Key set.
            memset(aes_->key, 0, KEY_SIZE);

            if(!RAND_bytes(aes_->key, KEY_SIZE)) {
                LOG(INFO)<<"RAND cannot random KEY";
            }

            //IV set
            memset(aes_->iv, 0, AES_BLOCK_SIZE);

            if(!RAND_bytes(aes_->iv, AES_BLOCK_SIZE)) {
                LOG(INFO)<<"RAND cannot random IV";
            }

            aes_->key_length = KEY_SIZE;

            //LOG(INFO)<<"Skip list size before insert : " << skip_list_accessor.size();

            skip_list_accessor.insert(std::unique_ptr<internet::security::aes_cbc>(
                    new internet::security::aes_cbc(ip, uuid, aes_->key, aes_->iv)
                    ));


            return aes_.get();
        }

        template<typename MessageType, typename EncryptType>
        bool aes_controller<MessageType, EncryptType>::find_key(std::string ip, std::string uuid)
        {
            boost::lock_guard<mutex_t> lock_(enc_mutex);

            typename aes_controller<MessageType, EncryptType>::
            skip_list_accessor_type skip_list_accessor(skip_list_ptr);

            std::unique_ptr<aes_cbc> aes = folly::make_unique<aes_cbc>(ip, uuid);

            if(skip_list_accessor.find(aes) != skip_list_accessor.end()) {
                return true;
            }

            return false;
        }

        template<typename MessageType, typename EncryptType>
        bool aes_controller<MessageType, EncryptType>::find_key(const char *ip, const char *uuid)
        {
            boost::lock_guard<mutex_t> lock_(enc_mutex);

            typename aes_controller<MessageType, EncryptType>::
            skip_list_accessor_type skip_list_accessor_find(skip_list_ptr);

            std::unique_ptr<aes_cbc> aes = folly::make_unique<aes_cbc>(ip, uuid);
            /*
            if(skip_list_accessor.find(aes) != skip_list_accessor.end()) {
            return true;
            }
            */
            //LOG(INFO)<<"Find key at IP : "<< ip <<", UUID : "<< uuid;

            return skip_list_accessor_find.contains(aes);//false;
        }

        template<typename MessageType, typename EncryptType>
        EncryptType *aes_controller<MessageType, EncryptType>::
        filter_key(const char *ip, const char *uuid)
        {

            boost::lock_guard<mutex_t> lock_(enc_mutex);

            typename aes_controller<MessageType, EncryptType>::
            skip_list_accessor_type skip_list_accessor(skip_list_ptr);

            std::unique_ptr<aes_cbc> aes = folly::make_unique<aes_cbc>(ip, uuid);
            skip_list_type::iterator iter_skip_list = skip_list_accessor.find(aes);
            /* : For case find with instance skip_list
            if(iter_skip_list != skip_list_accessor.end()) {

            if(*iter_skip_list == nullptr)
            return NULL;

            aes_cbc *aes = iter_skip_list->get();
            		LOG(INFO)<<"Search result : " << aes->ip;

            if(aes->ip == ip) {

            				LOG(INFO)<<"----------------------------------------------";
            LOG(INFO)<<"Found IP : " << aes->ip;
            LOG(INFO)<<"Search From IP : " << ip <<", UUID : " << uuid;
            				LOG(INFO)<<"----------------------------------------------";

            return aes;
            }

            }
            */

            //LOG(INFO)<<"Skip list size  : " << skip_list_accessor.size();

            //Support skip_list_accessor , Create: TBB parallel-for
            for(iter_skip_list = skip_list_accessor.begin();
                    iter_skip_list != skip_list_accessor.end();
                    ++iter_skip_list) {

                aes_cbc *aes = iter_skip_list->get();

                //LOG(INFO)<<"Search result : " << aes->ip<<", From IP : "<< ip <<", UUID : "<<uuid;

                if(aes->ip == ip) {

                    LOG(INFO)<<"Search result equal : " << aes->ip
                            <<", From IP : "<< ip
                            <<", UUID : "<<uuid
                            <<", Key  : "<<aes->key
                            <<", IV   : "<<aes->iv;

                    return  aes;
                }

            }

            return NULL;

        }//filter_key


        template<typename MessageType, typename EncryptType>
        bool aes_controller<MessageType, EncryptType>::
        encryption_msgs(const char *msg, int msg_length, EncryptType *aes)
        {
            boost::lock_guard<mutex_t> lock_(enc_mutex);


            int bytes_written_length = 0;

            aes->input_length = msg_length;

            EVP_CIPHER_CTX_init(&ctx_enc);
            EVP_EncryptInit_ex(&ctx_enc, EVP_aes_128_cbc(), NULL, aes->key, aes->iv);

            aes->input_length = msg_length + 1;
            aes->enc_msg = (unsigned char *)malloc(aes->input_length+ MAX_PADDING_LEN);

            EVP_EncryptUpdate(&ctx_enc,
                    aes->enc_msg,
                    &bytes_written_length,
                    (unsigned char *)msg,
                    aes->input_length);

            aes->enc_length += bytes_written_length;

            //LOG(INFO)<<"Input length : " << aes->input_length <<", Encrypt length : "<< aes->enc_length;

            EVP_EncryptFinal_ex(&ctx_enc,
                    aes->enc_msg + bytes_written_length,
                    &bytes_written_length);

            aes->enc_length += bytes_written_length;

            //LOG(INFO)<<"Input length : " << aes->input_length <<", Encrypt length : "<< aes->enc_length;

            return true;
        }

        //template<typename MessageType>
        template<typename MessageType, typename EncryptType>
        bool aes_controller<MessageType, EncryptType>::
        decryption_msgs(const char *msg, int msg_length, EncryptType *aes)
        {
						LOG(INFO)<<"Message decrypted start...";

            boost::lock_guard<mutex_t> lock_(enc_mutex);

            int bytes_written_length = 0;

            LOG(INFO)<<"AES, AES->MSG length :  "<< aes->enc_length;
            LOG(INFO)<<"AES, MSG length      :  "<< msg_length;
						LOG(INFO)<<"AES, Key             :  "<< aes->key;
						LOG(INFO)<<"AES, IV              :  "<< aes->iv;

            aes->dec_msg = (unsigned char *)malloc(msg_length);//(aes->enc_length);

            EVP_CIPHER_CTX_init(&ctx_dec);

            EVP_DecryptInit_ex(&ctx_dec, EVP_aes_128_cbc(), NULL, aes->key, aes->iv);

            EVP_DecryptUpdate(&ctx_dec,
                    aes->dec_msg,
                    &bytes_written_length,
                    aes->enc_msg,
                    msg_length);
            //aes->enc_length);

            aes->dec_length += bytes_written_length;



            EVP_DecryptFinal_ex(&ctx_dec,
                    aes->dec_msg + bytes_written_length,
                    &bytes_written_length);

            aes->dec_length += bytes_written_length;


            return true;

        }//decryption


        //string type
        template<typename MessageType, typename EncryptType>
        bool aes_controller<MessageType, EncryptType>::
        encryption_msgs(std::string msg, aes_cbc *aes)
        {
            boost::lock_guard<mutex_t> lock_(enc_mutex);

            int bytes_written_length = 0;

            aes->input_length = msg.size();

            EVP_CIPHER_CTX_init(&ctx_enc);
            EVP_EncryptInit_ex(&ctx_enc, EVP_aes_128_cbc(), NULL, aes->key, aes->iv);

            aes->input_length = msg.size() + 1;
            aes->enc_msg = (unsigned char *)malloc(aes->input_length+ MAX_PADDING_LEN);

            EVP_EncryptUpdate(&ctx_enc,
                    aes->enc_msg,
                    &bytes_written_length,
                    (unsigned char *)msg.data(),
                    aes->input_length);

            aes->enc_length += bytes_written_length;

            //LOG(INFO)<<"Input length : " << aes->input_length <<", Encrypt length : "<< aes->enc_length;

            EVP_EncryptFinal_ex(&ctx_enc,
                    aes->enc_msg + bytes_written_length,
                    &bytes_written_length);

            aes->enc_length += bytes_written_length;

            //LOG(INFO)<<"Input length : " << aes->input_length <<", Encrypt length : "<< aes->enc_length;
            //sync();

            return true;
        }//encryption

        //template<typename MessageType>
        template<typename MessageType, typename EncryptType>
        bool aes_controller<MessageType, EncryptType>::
        decryption_msgs(std::string msg, aes_cbc *aes)
        {

            boost::lock_guard<mutex_t> lock_(enc_mutex);

            LOG(INFO)<<"AES, DES-MSG Message size :  "<< aes->enc_length;

            int bytes_written_length = 0;

            aes->dec_msg = (unsigned char *)malloc(aes->enc_length);

            EVP_CIPHER_CTX_init(&ctx_dec);

            EVP_DecryptInit_ex(&ctx_dec, EVP_aes_128_cbc(), NULL, aes->key, aes->iv);

            EVP_DecryptUpdate(&ctx_dec,
                    aes->dec_msg,
                    &bytes_written_length,
                    aes->enc_msg,
                    aes->enc_length);

            aes->dec_length += bytes_written_length;



            EVP_DecryptFinal_ex(&ctx_dec,
                    aes->dec_msg + bytes_written_length,
                    &bytes_written_length);

            aes->dec_length += bytes_written_length;


            return true;

        }//decryption

        //lock mutex
        template<typename MessageType, typename EncryptType>
        void aes_controller<MessageType, EncryptType>::wait_sync()
        {
            //boost::unique_lock<boost::mutex> lock_(mutex_);
            //boost::lock_guard<mutex_t> lock_(enc_mutex);

            //sync_send = false;
            /*
            while(!sync_send) {
            cond_.wait(lock_);
            }//while
            */
        }

        template<typename MessageType, typename EncryptType>
        void aes_controller<MessageType, EncryptType>::sync()
        {
            //{
            //boost::lock_guard<mutex_t> lock_(enc_mutex);
            //    sync_send = true;
            //}
            //cond_.notify_all();
        }

        template<typename MessageType, typename EncryptType>
        aes_controller<MessageType, EncryptType>::
        ~aes_controller()
        {

            boost::lock_guard<mutex_t> lock_(enc_mutex);

            ENGINE_finish(engine_rdrand);
            ENGINE_free(engine_rdrand);
            ENGINE_cleanup();

        }//~

        template class aes_controller<message_scan::RequestScan, struct aes_cbc>;
        template class aes_controller<message_scan::ResponseScan, struct aes_cbc>;

    }//security

}//internet
