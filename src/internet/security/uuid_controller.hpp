#ifndef INTERNET_SECURITY_UUID_CONTROLLER_HPP
#define INTERNET_SECURITY_UUID_CONTROLLER_HPP

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

        //[-]UUID/Status  connection from client.
        /**
        * @brief UUID Controller is state controller when Server send/received message from client.
        * Solve problem not real client try to man in the middle.
        * UUID add last status of client in IObuffer-Queue.
        * Structure of data such
        *  [ IOBuffer-A ]   <- push([IOBuffer-A[ Buffer-Head...Buffer-Tail ]
        *  [ IOBuffer-B ]
        *       ...
        *       ...
        *  [ IOBuffer-N ]
        */
        template<typename MessageType = message_scan::RequestScan>
        class uuid_controller
        {
            public:

                typedef boost::shared_ptr<MessageType>  message_ptr_type;

                typedef internet::packedmessage_scan<message_scan::RequestScan> pack_msg_scan_type;

								typedef message_scan::RequestScan  msg_req_scan_type;

                uuid_controller();

                bool load_events_connection();

                /*Message request pointer */
                //typedef std::shared_ptr<MessageType> MessagePointer;
                /**
                * @brief  UUID send from client to re-check last status operates in server match or not.
                * Map<UUID, MessageType> map_uuid by key as UUID and value as MessagePointer.
                *
                * @param uuid UUID received from client.
                *
                * @return Messsage  of Request from client.
                */
                message_ptr_type get_msg_uuid(boost::shared_ptr<MessageType> msg_req);


                /**
                * @brief Initial buffer tail .
                *
                * @return Unique of IOBuffer when create with length of buffer name
                *  MESSAGE_PER_CONNECTION.
                */
                std::unique_ptr<folly::IOBuf> initial_iobuf();

                /**
                * @brief Append message to IOBuffer in head to tail of IO-Buffer.
                *
                * @param msg Message Type insert to IOBuffer.
                *
                * @return Success to insert message.
                */
                bool append_msg(MessageType msg);


								void append(std::unique_ptr<folly::IOBuf>& buf, std::string string);

                /**
                * @brief Add vector from row-encode client.
                *
                * @param msg Message not encode before send to IO-Buffer
                *
                * @return Data appeded to IO-Buffer completed.
                */
                bool append_msg(std::vector<uint8_t>  msg);

                /**
                 * @brief Add UUID to Database and status of UUID.
                *
                * @param uuid  UUID Send from Client.
                *
                * @return True status, if add UUID completed.
                */
                bool push_msg(std::unique_ptr<folly::IOBuf>  io_buf);


                /**
                * @brief Pop Message with UUID from queue.
                *
                * @return Message Type of push
                */
                std::unique_ptr<folly::IOBuf> pop_msg();


                int count_element()const;


                folly::IOBufQueue& get_queue();


            private:
                folly::IOBufQueue::Options cl_options;
                folly::IOBufQueue  iobuf_queue_;
								std::vector<std::unique_ptr<folly::IOBuf> >  io_buffer_vec;
                //buffer after encode in vector
                std::vector<uint8_t>  buffer_;

								boost::shared_ptr<MessageType> msg_req_;

                std::map<message_scan::ResponseScan::ResponseType, int>  events_response_map;
                std::map<message_scan::RequestScan::RequestType, int >  events_request_map;
        };

    }

}


#endif /* INTERNET_SECURITY_UUID_CONTROLLER_HPP */
