#ifndef INTERNET_SECURITY_3DES_CONTROLLER_HPP
#define INTERNET_SECURITY_3DES_CONTROLLER_HPP

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
 *- 3DES (EDE mode) encrypts buffer message communication between server/client.
 *                                                        R.Chatsiri
 */


namespace internet
{

    namespace security
    {

				template<typename MessageType>
        class 3des_controller
        {

            public:

                bool ping_service();

                bool find_key(std::string ip, std::string uuid);

                bool initial_key(std::string ip, std::string uuid);

							  std::vector<std::string> encryption_msgs(MessageType msgs);
							 
                std::map<uint8_t, std::string> get_symmetric_ede_key() const;

								std::string get_iv()const;

            private:

        };


    }

}

#endif /* INTERNET_SECURITY_3DES_CONTROLLER_HPP */
