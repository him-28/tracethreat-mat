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
#include "internet/msg/scan_server_client/message_network.pb.h"

namespace internet
{

    namespace security
    {

        //Security connection class call is call to security client service.
        //Manage all UUID and IP in Scan-server machanism.
        template<typename MessageNetworkType = message_network::ScanVirusInfo>
        class network_controller
        {

            public:
							//get mesage information.
							virtual	boost::shared_ptr<MessageNetworkType> get_nwinfo() = 0;
							//Name of class
           		virtual std::string get_name_system() = 0;
							//IP, UUID for system.
							//Use fast access such search . 
							virtual MessageNetworkType * filter_nwmsg(std::string uuid) = 0;

							//Store message of network.
							virtual bool store_nwmsg(MessageNetworkType & nwmsg) = 0; 
        };


				//Network service connects to tracethreat system.
				class network_service{

				};

    }

}


#endif /* INTERNET_SECURITY_CONNECTION_HPP */
