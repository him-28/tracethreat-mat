#include "internet/security/aes_controller.hpp"

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

//#include <iostream>

namespace internet
{

    namespace security
    {

        static void hex_print(const void *pv, size_t len)
        {
            const unsigned char *p = (const unsigned char *)pv;
            //const uint8_t *p = (const uint8_t *)pv;

            if (NULL == pv)
                printf("NULL");
            else {
                size_t i = 0;

                for (; i<len; ++i)
                    printf("%02X ", *p++);
            }

            printf("\n");
        }


        template<typename MessageType>
        bool aes_controller<MessageType>::initial_engine()
        {
            //Random engine.
            /*
            engine_rdrand = ENGINE_by_id("rdrand");

            //engine_encrypts = ENGINE_by_id("encryption");
            if(engine_rdrand == NULL) {
                return false;
            }

            if(!ENGINE_init(engine_rdrand)) {
                return false;
            }

            if(!ENGINE_set_default(engine_rdrand, ENGINE_METHOD_RAND)) {
                return false;
            }
            			*/
            return true;

        }

        template<typename MessageType>
        internet::security::aes_cbc *aes_controller<MessageType>::
        initial_key(std::string ip, std::string uuid)
        {
            typename aes_controller<MessageType>::skip_list_accessor_type skip_list_accessor(skip_list_ptr);
            {
                //std::unique_ptr<aes_cbc> aes_ptr = folly::make_unique<aes_cbc>(ip, uuid);
                aes_ptr = new aes_cbc(ip,uuid);
                LOG(INFO)<<"Initial AES key for client IP : "<< aes_ptr->ip;
                LOG(INFO)<<"Initial UUID                  : "<< aes_ptr->uuid;
                //Key set.
                memset(aes_ptr->key, 0, KEY_SIZE);

                if(!RAND_bytes(aes_ptr->key, KEY_SIZE)) {
                    LOG(INFO)<<"RAND cannot random KEY";
                }

                //IV set
                memset(aes_ptr->iv, 0, AES_BLOCK_SIZE);

                if(!RAND_bytes(aes_ptr->iv, KEY_SIZE)) {
                    LOG(INFO)<<"RAND cannot random IV";
                }

                //memcpy(aes_ptr->iv, aes_ptr->iv, AES_BLOCK_SIZE);

                aes_ptr->key_length = KEY_SIZE;

                skip_list_accessor.insert(std::unique_ptr<aes_cbc>(aes_ptr));

                return aes_ptr;

            }
        }

        template<typename MessageType>
        bool aes_controller<MessageType>::find_key(std::string ip, std::string uuid)
        {
            typename aes_controller<MessageType>::skip_list_accessor_type skip_list_accessor(skip_list_ptr);

            std::unique_ptr<aes_cbc> aes = folly::make_unique<aes_cbc>(ip, uuid);

            if(skip_list_accessor.find(aes) == skip_list_accessor.end()) {
                return true;
            }

            return false;
        }

        //encrypt message
        template<typename MessageType>
        bool aes_controller<MessageType>::
        encryption_msgs(std::vector<uint8_t>& msg_vec, aes_cbc *aes)
        {
            /*
            						AES_KEY enc_key;

            						if(msg_vec.empty())
            							return false;


                        aes->input_length = msg_vec.size();
                        aes->enc_length = ((msg_vec.size()+ AES_BLOCK_SIZE)/ AES_BLOCK_SIZE)* AES_BLOCK_SIZE;
                        aes->enc_msg = new uint8_t[aes->enc_length];

                        memset(aes->enc_msg, 0, sizeof(aes->enc_msg));


                        AES_set_encrypt_key(aes->key, KEY_ENCRYPTION_SIZE, &enc_key);

                        LOG(INFO)<<" Encryption message IP : "<< aes->ip <<", vector size : "<< msg_vec.size();
                        LOG(INFO)<<" Encryption length : " << aes->enc_length;

            						printf("Key Encryption : \t");
                        hex_print((uint8_t *)&aes->key, KEY_SIZE);
            						printf("IV  Encryption : \t");
                        hex_print((uint8_t *)&aes->iv, AES_BLOCK_SIZE);

                        AES_cbc_encrypt((uint8_t *)&msg_vec.front(),
                                aes->enc_msg,
                                aes->enc_length,
                                &enc_key,
                                aes->iv,
                                AES_ENCRYPT);

                        LOG(INFO)<<"Encryption success";
            */
            return true;
        }//encryption

        template<typename MessageType>
        bool aes_controller<MessageType>::
        decryption_msgs(std::vector<uint8_t>& msg_vec, aes_cbc *aes)
        {
            /*
            						if(aes->input_length == 0)
            								return false;

            					printf("Message encryption input to decrypt in hex : \t");
            		      hex_print((uint8_t *)&aes->enc_msg, aes->enc_length);
            					//printf("Message Input length : \t");
            		      //hex_print((unsigned char *)&msg_vec, aes->input_length);

                        //set key
                        AES_set_decrypt_key(aes->key, KEY_ENCRYPTION_SIZE, &dec_key);

            						aes->dec_msg = new uint8_t[aes->input_length];

            						msg_vec.resize(aes->input_length);

                        AES_cbc_encrypt(aes->enc_msg,
                                &msg_vec.front(),
                                aes->enc_length,
                                &dec_key,
                                aes->iv,
                                AES_DECRYPT);

            						printf("Message after Decrypt hex : \t");
                        hex_print((uint8_t *)&msg_vec.front(), msg_vec.size());

            						//printf("Message after Decrypt hex : \t");
                        //hex_print((uint8_t *)aes->dec_msg, msg_vec.size());

            						//hex_print((unsigned char *)&aes->dec_msg, aes->input_length);
            */
            return true;

        }//decryption


        //encrypt message
        template<typename MessageType>
        bool aes_controller<MessageType>::
        encryption_msgs(boost::shared_ptr<MessageType> msg_vec, aes_cbc *aes)
        {
            /*
            aes->input_length = msg_vec->ByteSize();
            aes->enc_length = ((msg_vec->ByteSize()+ AES_BLOCK_SIZE)/ AES_BLOCK_SIZE)* AES_BLOCK_SIZE;

            //aes->enc_msg = new unsigned char[aes->enc_length];
            //(unsigned char*)malloc(sizeof(unsigned char) * aes->enc_length);

            unsigned char enc_msg[aes->enc_length];
            memset(enc_msg, 0, sizeof(enc_msg));


            printf("Encryption before  send to cbc : \t");
            hex_print((unsigned char *)&enc_msg, aes->enc_length);

            LOG(INFO)<<" aes->enc_msg, size : " << sizeof(aes->enc_msg);

            //set key
            AES_set_encrypt_key(aes->key, KEY_ENCRYPTION_SIZE, &enc_key);

            LOG(INFO)<<" Encryption message IP : "<< aes->ip <<", vector size : "<< msg_vec->ByteSize();
            LOG(INFO)<<" Encryption length : " << aes->enc_length;

            printf("Key Encryption : \t");
            hex_print((unsigned char *)&aes->key, KEY_SIZE);

            printf("IV  Encryption : \t");
            hex_print((unsigned char *)&aes->iv, AES_BLOCK_SIZE);

            aes->msg = (unsigned char *)malloc(sizeof(unsigned char) * msg_vec->ByteSize());

            memset(aes->msg, 0, sizeof(aes->msg));

            msg_vec->SerializeToArray(&aes->msg, msg_vec->ByteSize());

            printf("Message  Encrypted : \t");
            hex_print((unsigned char *)&aes->msg, msg_vec->ByteSize());


            AES_cbc_encrypt((unsigned char *)&aes->msg,
            enc_msg,
            aes->enc_length,
            &enc_key,
            aes->iv,
            AES_ENCRYPT);

            LOG(INFO)<<" Enc_msg size : " << sizeof(enc_msg);

            aes->enc_msg = enc_msg;

            printf("Message encryption in hex : \t");
            hex_print((unsigned char *)&aes->enc_msg, aes->enc_length);

            LOG(INFO)<<"Encryption success";
            */
            return true;
        }//encryption

        template<typename MessageType>
        bool aes_controller<MessageType>::
        decryption_msgs(boost::shared_ptr<MessageType> msg_vec, aes_cbc *aes)
        {
            /*
            if(aes->input_length == 0)
            return false;

            printf("Message encryption input in hex : \t");
            hex_print((unsigned char *)&aes->enc_msg, aes->enc_length);

            //set key
            AES_set_decrypt_key(aes->key, KEY_ENCRYPTION_SIZE, &dec_key);

            unsigned char dec_msg[aes->input_length];

            //aes->dec_msg = new unsigned char[aes->input_length];

            memset(dec_msg, 0, sizeof(dec_msg));

            printf("Message dec hex : \t");
            hex_print((unsigned char *)dec_msg, aes->input_length);


            AES_cbc_encrypt(aes->enc_msg,
            dec_msg,
            aes->enc_length,
            &dec_key,
            aes->iv,
            AES_DECRYPT);

            printf("Message after Decrypt hex : \t");
            hex_print((unsigned char *)dec_msg, aes->input_length);

            msg_vec->ParseFromArray(dec_msg, aes->input_length);

            LOG(INFO)<<" UUID dec : " << msg_vec->uuid();
            */
            return true;

        }//decryption

        //encrypt message
        template<typename MessageType>
        bool aes_controller<MessageType>::
        encryption_msgs(const char   *msg, std::size_t length, aes_cbc *aes)
        {
            /*
            aes->input_length = length;
            aes->enc_length = ((length+ AES_BLOCK_SIZE)/ AES_BLOCK_SIZE)* AES_BLOCK_SIZE;

            unsigned char enc_msg[aes->enc_length];
            memset(enc_msg, 0, sizeof(enc_msg));


            printf("Message before encrypt send to cbc : \t");
            hex_print((unsigned char *)&enc_msg, aes->enc_length);

            //set key
            AES_set_encrypt_key(aes->key, KEY_ENCRYPTION_SIZE, &enc_key);

            LOG(INFO)<<" Encryption message IP : "<< aes->ip
            <<", Message size : "<< length
            <<", Message Encryption length : "<< aes->enc_length;

            printf("Key Encryption : \t");
            hex_print((unsigned char *)&aes->key, KEY_SIZE);

            printf("IV  Encryption : \t");
            hex_print((unsigned char *)&aes->iv, AES_BLOCK_SIZE);

            aes->msg = new unsigned char[length];
            memset(aes->msg, 0, sizeof(aes->msg));
            memcpy(aes->msg, msg, length);

            printf("Plain text : \t");
            hex_print((unsigned char *)msg, length);

            printf("Convert msg to aes->msg  : \t");
            hex_print((unsigned char *)aes->msg, length);

            AES_cbc_encrypt(aes->msg,
            enc_msg,
            aes->enc_length,
            &enc_key,
            aes->iv,
            AES_ENCRYPT);

            aes->enc_msg = new unsigned char[aes->enc_length];
            memset(aes->enc_msg, 0,  sizeof(aes->enc_msg));
            memcpy(aes->enc_msg, enc_msg, aes->enc_length);

            printf("Message encryption in hex : \t");
            hex_print((unsigned char *)&enc_msg, aes->enc_length);


            printf("Message convert to encryption in hex : \t");
            hex_print((unsigned char *)aes->enc_msg, aes->enc_length);

            LOG(INFO)<<"Encryption success";
            return true;
            }//encryption

            template<typename MessageType>
            bool aes_controller<MessageType>::
            decryption_msgs(const char *msg, std::size_t length, aes_cbc *aes)
            {

            if(aes->input_length == 0)
            return false;

            printf("Message encryption input in hex : \t");
            hex_print((unsigned char *)aes->enc_msg, aes->enc_length);

            unsigned char dec_msg[aes->input_length];
            memset(dec_msg, 0, sizeof(dec_msg));


            //set key
            AES_set_decrypt_key(aes->key, KEY_ENCRYPTION_SIZE, &dec_key);

            printf("Message decryption hex : \t");
            hex_print((unsigned char *)dec_msg, aes->input_length);

            AES_cbc_encrypt(aes->enc_msg,
            dec_msg,
            aes->enc_length,
            &dec_key,
            aes->iv,
            AES_DECRYPT);


            aes->dec_msg = new unsigned char[aes->input_length];
            memset(aes->dec_msg, 0, sizeof(aes->dec_msg));
            memcpy(aes->dec_msg, dec_msg, aes->input_length);

            printf("Message after Decrypt hex : \t");
            hex_print((unsigned char *)dec_msg, aes->input_length);

            printf("Message after Decrypt hex : \t");
            hex_print((unsigned char *)&aes->dec_msg, aes->input_length);
            */
            return true;

        }//decryption

        //string type
        template<typename MessageType>
        bool aes_controller<MessageType>::
        encryption_msgs(std::string msg, aes_cbc *aes)
        {
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

            LOG(INFO)<<"Input length : " << aes->input_length <<", Encrypt length : "<< aes->enc_length;

            EVP_EncryptFinal_ex(&ctx_enc,
                    aes->enc_msg + bytes_written_length,
                    &bytes_written_length);

            aes->enc_length += bytes_written_length;

            LOG(INFO)<<"Input length : " << aes->input_length <<", Encrypt length : "<< aes->enc_length;

            /*
            EVP_EncryptUpdate(&ctx,
            (unsigned char *)aes->enc_msg,
            &aes->enc_length,
            (unsigned char *)msg.data(),
            aes->input_length );

            EVP_EncryptFinal(&ctx,
            (unsigned char *)&aes->enc_msg[aes->enc_length],
            &aes->enc_length);
            */

            //LOG(INFO)<<"Encrypted message : " <<aes->enc_msg;

            return true;
        }//encryption

        template<typename MessageType>
        bool aes_controller<MessageType>::
        decryption_msgs(std::string msg, aes_cbc *aes)
        {

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


            /*
            aes->dec_msg  = new unsigned char[aes->input_length];

            EVP_DecryptInit(&ctx, EVP_aes_128_cbc(), aes->key, aes->iv);

            EVP_DecryptUpdate(&ctx,
            (unsigned char *)aes->dec_msg,
            &aes->dec_length,
            (unsigned char*)aes->enc_msg,
            aes->enc_length);

            EVP_DecryptFinal(&ctx,
            (unsigned char *)&aes->dec_msg,
            &aes->output_length);

            LOG(INFO)<<"Decrypted message : " << aes->dec_msg;
            */
            return true;

        }//decryption






        template class aes_controller<message_scan::RequestScan>;
        template class aes_controller<message_scan::ResponseScan>;

    }//security

}//internet
