#ifndef INTERNET_SECURITY_CONNECTION_HPP
#define INTERNET_SECURITY_CONNECTION_HPP

/* Intial message size per IOBuffer */
#define MESSAGE_PER_CONNECTION 5

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
 * - UUID Controller : Queue per thread of client connection.
 * - Encryption/Decryption Controller :
 * - Certificate Controller :                             R.Chatsiri
 */
#include <memory>
#include <map>

// UUID Queue support per thread.
#include <folly/io/IOBufQueue.h>

//#include "internet/msg/security_client/message_scan.pb.h"

#include "internet/msg/packedmessage_scan.hpp"

namespace internet
{

    namespace security
    {

        //Stage-2 After exchanged 3DES on SSL
        //[-]Encrypt/Decrypt message in second step.
        //3DES
        template<typename MessageType>
        class crypto_controller
        {
            public:

                const char *getKey(const char *uuid)const;

                bool encrypt_msg(const char *plain_text);

                const char *get_encrypted_msg() const;

                bool decrypt_msg(const char *cipher_text);

                const char *get_decrypted_msg() const;

            private:
                std::string key_mutalble_3des;
                MessageType plain_text_ptr;

        };


        //[-]Store Key and certificate connection between Server/Client.
        class certificate_controller
        {

            public:


            private:

        };



    }

}


#endif /* INTERNET_SECURITY_CONNECTION_HPP */
