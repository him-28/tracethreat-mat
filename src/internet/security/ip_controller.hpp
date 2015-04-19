#ifndef INTERNET_SECURITY_IP_CONTROLLER_HPP
#define INTERNET_SECURITY_IP_CONTROLLER_HPP

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
 * - Report IP not permission to system. Messages report send to mailbox.(Tracethreat).
 * - Support internal network with TCP/IP Asynchronous
 * - Support Mulithread server(R/W).
 *                                                        R.Chatsiri       07/12/2014
 *
 */

#include "folly/AtomicHashMap.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>

#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include <rocksdb/options.h>

#include <memory>

namespace internet
{

    namespace security
    {


        //Class IP_incident_controller will check IP report from UIID controller.
        //RockDB record in passive store and IP_Incident_controller load to memory when
        //UUID controller want to check IP.
        //[...Server Scan...]    --->   [Mailbox-Tracethreat]
        //       ||              |
        //[..IP_controller..]-----(Ping,Set timeout, Sending/Receiving)
        //
        template<typename MessageType>
        class ip_controller
        {

            public:

                /**
                * @brief Ping service from Tracethreat service.(Internal Ping).
                *
                * @return True,If tracethreat open mailbox.
                */
                bool ping_service();

                /**
                * @brief Message send to tracethreat service for tracing IP.
                *
                * @param msg  Message such message_scan::Request and message_scan::Response
                */
                void block_msg_send(std::unique_ptr<MessageType> msg);

                /**
                * @brief IP_controller received success/unsuccess message from Tracethreat.
                */
                void block_msg_receive();

                /**
                * @brief IP Loaded from RocksDB.
                */
                bool load_ip();

                /**
                * @brief Filter IP Address from client. When Scan_server accepts connection from client.
                *
                * @param ip  IP from folly::IPAddress. Format 192.168.0.1
                * @param msg_ptr  Object of message_scan::Reqeust.
                *
                * @return False if found IP in database. IP reported by UUID_controller class.
                */
                bool filter_ip(const  char *ip, std::unique_ptr<MessageType>& msg_ptr);

                /**
                * @brief IP and Serializable object of message_scan::Request write to database.
                *
                * @param ip IP from folly::IPAddress. Format 192.168.0.1.
                * @param msg_ptr Object of message_scan::Reqeust.
                *
                * @return Insert object success or not.
                */
                bool store_ip(const  char   *ip, std::unique_ptr<MessageType>& msg_ptr);

                /**
                * @brief Wait process jobs in sync state.
                */
                void wait_sync();

                /**
                * @brief Sync after process procced jobs.
                */
                void sync();

            private:

								bool sync_send;

                boost::mutex mutex_;
                boost::condition_variable cond_;

                folly::AtomicHashMap<int64_t, std::string> ip_map;

                const unsigned char *ip_;

                rocksdb::DB *db;

                rocksdb::Options options;

                std::string db_path;
        };


    }


}


#endif /* INTERNET_SECURITY_IP_CONTROLLER_HPP */
