#define INTERVAL_TYPE_NON_SEQ 0
#define INTERVAL_TYPE_SEQ 1

#define MESSAGE_INITIAL_SIZE 1024
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
 * - Initial Queue Buffer for UUID_Controller.            R.Chatsiri
 *   * Use queue because plan to adaptes multiple check .
 * - The last line of defends after filtering IP & User connection to ServerScan system.
 *                                                        R.Chatsiri
 */

#include <boost/shared_ptr.hpp>

#include "internet/scan_server/security_connection.hpp"

namespace internet
{

    namespace security
    {
        template<typename MessageType>
        uuid_controller<MessageType>::uuid_controller():
            iobuf_queue_(folly::IOBufQueue::cacheChainLength())
        {
            load_events_connection();

            if(!events_request_map.empty()) {
                LOG(INFO)<<"UUID Controller not Events mapping, Start initial Events";

                std::map<message_scan::RequestScan::RequestType, int>::iterator iter_events;

                for(iter_events = events_request_map.begin();
                        iter_events != events_request_map.end();
                        ++iter_events) {
                    std::pair<message_scan::RequestScan::RequestType,int> events = *iter_events;
                    LOG(INFO)<<"UUID Event contained No. : "<<events.second;
                }
            }
        }

        template<typename MessageType>
        bool uuid_controller<MessageType>::load_events_connection()
        {
            events_request_map.insert(std::make_pair(message_scan::RequestScan::REGISTER, 1));
            events_request_map.insert(std::make_pair(message_scan::RequestScan::SCAN, 2));
            events_request_map.insert(std::make_pair(message_scan::RequestScan::CLOSE_CONNECTION,3));
        };


        /**
        * @brief Search UUID from QueueList in thread. Queue will contains last event message from client
        * call to Server scan.
        *
        * @param uuid  UUID number of queue.
        *
        * @return Message of uuid_controller type.
        */
        template<typename MessageType>
        typename uuid_controller<MessageType>::message_ptr_type
        uuid_controller<MessageType>::get_msg_uuid(boost::shared_ptr<MessageType> msg_req)
        {
            //msg_req_ = msg_req;

						LOG(INFO)<<"Message external UUID : " << msg_req->uuid();
						LOG(INFO)<<"Message external Type : " << msg_req->type();

            //Not initial UUID
            if(msg_req->uuid().empty()) {
                LOG(INFO)<<"Fraud Message access to system, Information write to mailbox";
                message_ptr_type msg_type(new MessageType);
                return msg_type;
            }

            //Check type scanning in message from client.
            uint8_t new_type = msg_req->type();

            //Check message type from client.
            std::map<message_scan::RequestScan::RequestType, int>::iterator iter_msg_req_map;
            //Find message new type in events_request_map.
            iter_msg_req_map = events_request_map.find(msg_req->type());

            if(iter_msg_req_map != events_request_map.end()) {


            } else {
                //IP Incident response to database
                //[-]Add Fraud message to tracetreat system.

            }//else

            //[-] Get front queue check
            //Queue iobuffer [...UUID-A...]
            //               [...UUID-B...] <-- [...UUID-B-SCAN...]
            if(io_buffer_vec.size() == 0) {

                if(message_scan::RequestScan::REGISTER == new_type) {
                    //[-]Add new message-UUID to queue.

                    std::unique_ptr<folly::IOBuf>  iobuffer_front =
                            folly::IOBuf::create(MESSAGE_INITIAL_SIZE);

                    std::string msg_parse;

                    msg_req->SerializeToString(&msg_parse);

                    append(iobuffer_front, msg_parse);

                    io_buffer_vec.push_back(std::move(iobuffer_front));

                    LOG(INFO)<<"Register message with UUID : "<<  msg_req->uuid();
                    return msg_req;
                } else {

                    //[-]Add Fraud message to tracetreat system.
                     LOG(INFO)<<"------------------------------------------------------------";
                     LOG(INFO)<<"Fraud Message access to system, Information write to mailbox";
										 LOG(INFO)<<" UUID : " << msg_req->uuid();
										 LOG(INFO)<<" Type : " << msg_req->type();
										 LOG(INFO)<<"------------------------------------------------------------";

                    return msg_req;
                }

            }//if


            int io_buf_size = io_buffer_vec.size();
            int count_tail  = 1;

						for(auto& msg_iobuffer : io_buffer_vec) {

                std::string msg_internal = std::string(reinterpret_cast<const char *>(msg_iobuffer->data()), 																												msg_iobuffer->length());

                message_ptr_type  msg_req_scan_(new message_scan::RequestScan);

                msg_req_scan_->ParseFromString(msg_internal);
								
								LOG(INFO)<<"--------------------------------------------------";
                LOG(INFO)<<"Message Internal UUID : " << msg_req_scan_->uuid();
			          LOG(INFO)<<"Message internal Type : " << msg_req_scan_->type();

                LOG(INFO)<<"Message External UUID : " << msg_req->uuid();
				        LOG(INFO)<<"Message External Type : " << msg_req->type();

                //Found UUID and check type() of message
                if(!msg_req_scan_->uuid().compare(msg_req->uuid())) {

                    uint8_t old_type = msg_req_scan_->type();

                    int interval_type  = new_type - old_type;

                    LOG(INFO)<<"New type : "<<(int)new_type
														 <<", Internal type : "<<(int)old_type
                             <<", Interval type : " << interval_type;

                    if((interval_type == INTERVAL_TYPE_SEQ) && (count_tail == io_buf_size) ) {

                        std::unique_ptr<folly::IOBuf>  iobuffer =
                                folly::IOBuf::create(MESSAGE_INITIAL_SIZE);

                        std::string msg_parse;

                        msg_req->SerializeToString(&msg_parse);

                        append(iobuffer, msg_parse);

                        io_buffer_vec.push_back(std::move(iobuffer));

												LOG(INFO)<<"Message external register completed with Type : "<< msg_req->type();

                    } else if(interval_type == INTERVAL_TYPE_NON_SEQ) {
                        //If send of message such : CLOSE_CONNECTION is normal case.
                        if(msg_req->type() == message_scan::RequestScan::CLOSE_CONNECTION) {
                            //[-]close duplicate.
                        } else {
                            //[-]IP Incident response
                            //[-]Fraud message from anonymous client.
                            //[-]Add fraud message to tracetreat system.(Fraud status)
                            LOG(INFO)<<"------------------------------------------------------------";
                            LOG(INFO)<<"Fraud Message access to system, Information write to mailbox";
													  LOG(INFO)<<" UUID : " << msg_req->uuid();
														LOG(INFO)<<" Type : " << msg_req->type();
														LOG(INFO)<<"------------------------------------------------------------";
                        }

                    } else {

                        //check next message
                    }//else

                } else {

                    if(new_type != message_scan::RequestScan::REGISTER) {
                        //[-]IP incedent report for not connection to internal system.
                            LOG(INFO)<<"------------------------------------------------------------";
                            LOG(INFO)<<"Fraud Message access to system, Information write to mailbox";
													  LOG(INFO)<<" UUID : " << msg_req->uuid();
														LOG(INFO)<<" Type : " << msg_req->type();
														LOG(INFO)<<"------------------------------------------------------------";

                    } else {
                        LOG(INFO)<<" Clear on message on container ";
                        //[-]Add old message to tracetreat system. (Old message status)
                        //[-]Deleted old message-UUID in queue of UUID_controller
												io_buffer_vec.clear();
                        //[-]Add new message-UUID to vector.
			                  std::unique_ptr<folly::IOBuf>  iobuffer_front =
                                folly::IOBuf::create(MESSAGE_INITIAL_SIZE);

                        std::string msg_parse;

                        msg_req->SerializeToString(&msg_parse);

                        append(iobuffer_front, msg_parse);

                        io_buffer_vec.push_back(std::move(iobuffer_front));


                    }//else

                }//else

                count_tail++;

								LOG(INFO)<<"--------------------------------------------------";

            }//for

            //Step write from vector back to queue.
            return msg_req;
        }

        template<typename MessageType>
        void uuid_controller<MessageType>::append(std::unique_ptr<folly::IOBuf>& buf, std::string str)
        {
            memcpy(buf->writableData(), str.data(), str.size());
            buf->append(str.size());
        }


        template<typename MessageType>
        bool uuid_controller<MessageType>::push_msg(std::unique_ptr<folly::IOBuf>  io_buf)
        {
            iobuf_queue_.append(std::move(io_buf));
        }

        template<typename MessageType>
        std::unique_ptr<folly::IOBuf> uuid_controller<MessageType>::initial_iobuf()
        {
            std::unique_ptr<folly::IOBuf> buf = folly::IOBuf::create(MESSAGE_INITIAL_SIZE);
            return std::move(buf);
        }

        template<typename MessageType>
        bool uuid_controller<MessageType>::append_msg(std::vector<uint8_t> msg_vec)
        {
            //write message to IO-Buffer
        }

        template<typename MessageType>
        bool uuid_controller<MessageType>::append_msg(MessageType msg)
        {
            //convert from MessageType to std::vector<uint8_t>
        }




        template<typename MessageType>
        int uuid_controller<MessageType>::count_element()const
        {
            return iobuf_queue_.chainLength();
        }

        template<typename MessageType>
        folly::IOBufQueue& uuid_controller<MessageType>::get_queue()
        {
            return iobuf_queue_;
        }

        template class uuid_controller<message_scan::RequestScan>;

    }

}
