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

//ASIO Network system.
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <iostream>

//Parser string
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

//funstion with structure
#include <boost/fusion/include/struct.hpp>
#include <boost/fusion/include/nview.hpp>
#include <boost/foreach.hpp>
#include <boost/mpl/print.hpp>
//progress bar
#include <boost/progress.hpp>

#include "internet/logger/logging.hpp"

#define FILE_BUFFER 8192
// 8192

namespace internet
{

    //packet header
    struct http_header {
        std::string type;
        std::string value;
    };


    using boost::spirit::qi::char_;
    using boost::spirit::qi::double_;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;

    namespace asio = boost::asio;
    typedef asio::ip::tcp protocol;
    typedef protocol::resolver resolver;
    typedef protocol::socket socket;
    typedef boost::system::error_code error_code;

    class updated_sign;

    //[x] DNS.
    //[x] Read db file for repository.
    //[] Refectory
    class updated_sign
    {
        public:

            typedef updated_sign self_type;

            typedef boost::fusion::result_of::as_nview<http_header, 0, 1>::type http_header_type;

            typedef std::string::const_iterator iterator_type;

            void repos_initial(std::string ip_addr, std::string port);

            //Connect headler
            void connect_handler(const error_code& error, socket *sock);

            //Read data data.
            void read_headler(const error_code& error, std::size_t bytes_transffered);

            char *get_version() {
                return "devel-5bf764d-exp-debug";
            }

            //Write signature base to file.
            bool write_sign(std::vector<char> *buffer_vec);

            void set_filepath(const char *file_path) {
                this->file_path = file_path;
                LOG(INFO)<<" File Path contain signature-db : "<< this->file_path;
            }

            char set_srcfile(const char *srcfile) {
                this->srcfile = srcfile;
            }

            char set_remotename(const char *remotename) {
                this->remotename = remotename;
            }

            bool parse_header_cvd(std::string const& header_str,
                    std::vector<http_header *>& header_vector,
                    uint8_t count_header);

            void on_read_header(const error_code& error, size_t bytes, socket * sock);

            void read_header(const error_code& error, socket *sock);

            void on_read_body(const error_code& error, size_t bytes);

            size_t read_body_completion(const error_code & error, size_t bytes);

        private:

            asio::io_service io_service;
            // Write command to socket.
            char cmd[512];
            const char *remotename;
            const char *srcfile;
            char *cvdfile[32];
            char uastr[128];
            char *last_modifield[36];

            //Max buffer size
            enum { max_buff = FILE_BUFFER };

            //Read buffer declare
            char read_buffer_[max_buff];

            std::vector<char>   buffer_vec;
            const char *file_path;

            //rule parser
            boost::spirit::qi::rule<iterator_type, http_header_type()> rule_http_header;
            //Progress bar;
            boost::progress_display *show_progress;

    };



}

#endif /* INTERNET_UPDATED_SIGN_HPP */
