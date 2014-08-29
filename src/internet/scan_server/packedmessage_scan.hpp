#ifndef INTERNET_PACKEDMESSAGE_SCAN_HPP
#define INTERNET_PACKEDMESSAGE_SCAN_HPP
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
 *  - Packed Message and Compression file from communication between client and server.
 *                                                        R.Chatsiri
 */
#define HEADER_SIZE 4

#include <boost/shared_ptr.hpp>

#include "internet/logger/logging.hpp"

#include "internet/msg/scan_server_client/message_scan.pb.h"

namespace internet
{
		typedef std::vector<uint8_t> data_buffer;

    template<typename MessageType>
    class packedmessage_scan
    {
            // Send 1 Pack per group of file name.
        public:
            typedef boost::shared_ptr<MessageType> message_pointer;

            packedmessage_scan(message_pointer msg = message_pointer()) : msgs(msg) { }
					
						unsigned decode_header(const data_buffer & buffer)const;

						bool unpack(const data_buffer & buffer);
	
						message_pointer get_msg();
						
        private:
            message_pointer  msgs;
    };


}


#endif /* INTERNET_PACKEDMESSAGE_SCAN_HPP */
