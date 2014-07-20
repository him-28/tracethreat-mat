#ifndef INTERNET_UPDATED_SIGN_HPP
#define INTERNET_UPDATED_SIGN_HPP
/*
* Copyright 2014 MTSec, Inc.
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
 * - Support load signature to system. Boost.ASIO         R.Chatsiri       15/07/2014
 */
#include <boost/asio.hpp>
#include <iostream>

namespace internet
{
    namespace asio = boost::asio;
    typedef asio::ip::tcp protocol;
    typedef protocol::resolver resolver;
    typedef protocol::socket socket;
    typedef boost::system::error_code error_code;


    //[] DNS.
    //[] Read db file for repository.
    //[] Refectory
    class updated_sign
    {
        public:
            void repos_initial(std::string ip_addr, std::string port);

            //Connect headler
            void connect_handler(const error_code & error, socket * sock);
            //Read data data.
            void read_headler(const error_code& error, std::size_t bytes_transffered);
            //Write signature base to file.
            void write_sig(const char *buffer_write);
				
						char * get_version(){ return "devel-5bf764d-exp-debug"; }

						char set_srcfile(const char * srcfile){ this->srcfile = srcfile; }	

						char set_remotename(const char * remotename){ this->remotename = remotename; }

						bool parse_header_cvd(const char *  header);
						
        private:

            asio::io_service io_service;
						// Write command to socket.
            char cmd[512];
					  const char * remotename;
						const char * srcfile;
						char * cvdfile[32];
						char uastr[128];
						char * last_modifield[36];
						
    };



}

#endif /* INTERNET_UPDATED_SIGN_HPP */
