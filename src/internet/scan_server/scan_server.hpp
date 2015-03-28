#ifndef INTERNET_SERVER_HANDLER_HPP
#define INTERNET_SERVER_HANDLER_HPP

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
 *  - Server with protobuffer.                            R.Chatsiri       27/07/2014
 */

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "internet/logger/logging.hpp"

namespace internet
{

    namespace asio = boost::asio;

    class scan_server
    {

        public:
            //typedef boost::shared_ptr<asio::io_service> &  io_service_type;

            typedef asio::io_service&  io_service_type;

            scan_server(io_service_type io_service,
                    std::string ip_addr,
                    unsigned port,
                    const char *file_path,
                    std::string tracethreat_ip,
                    int tracethrat_port);

            ~scan_server();

        private:
            scan_server();

            void start_accept();

            //Factory class.
            class impl;

            boost::shared_ptr<impl> impl_;

    };

}




#endif /* INTERNET_SERVER_HANDLER_HPP */
