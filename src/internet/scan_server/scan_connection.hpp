#ifndef INTERNET_SCAN_CONNECTION_HPP
#define INTERNET_SCAN_CONNECTION_HPP

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
 * Server connection                                      R.Chatsiri       30/07/2014
 */

//#define HEADER_SIZE 4

#include <boost/thread.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <vector>

#include "scan/scan_file_controller.hpp"

#include "internet/utils/common.hpp"

#include "internet/logger/logging.hpp"

#include "internet/msg/scan_server_client/message_scan.pb.h"
#include "internet/scan_server/packedmessage_scan.hpp"

namespace internet
{
    namespace asio = boost::asio;

    class scan_connection	 : public boost::enable_shared_from_this<scan_connection>
    {

        public:
            typedef boost::shared_ptr<scan_connection> pointer;
            typedef boost::shared_ptr<message_scan::RequestScan>  MsgsRequestPointer;
            typedef boost::shared_ptr<message_scan::ResponseScan> MsgsResponsePointer;

            static pointer create(asio::io_service& io_service) {

                return scan_connection::pointer(new scan_connection(io_service));

            }//, message_scan& msgs);

            asio::ip::tcp::socket& get_socket();

            void start();


        private:

            asio::ip::tcp::socket msgs_socket;
            //message_scan& msgs_scan_ref;
            std::vector<uint8_t> msgs_read_buffer;

            packedmessage_scan<message_scan::RequestScan> msgs_packed_request_scan;

            std::map<std::string, file_detail_scan> fd_scan_map;

            scan_connection(asio::io_service& io_service) :
                msgs_socket(io_service),
                msgs_packed_request_scan(boost::shared_ptr<message_scan::RequestScan>(
                        new message_scan::RequestScan())) {


            }

            void handle_read_header(const boost::system::error_code& error, std::size_t bytes);

            void handle_read_body(const boost::system::error_code& error);

 
            typename scan_connection::MsgsResponsePointer
            prepare_response_register(MsgsRequestPointer);


						//Response
						void write_response(MsgsRequestPointer  request_ptr, MsgsResponsePointer  response_ptr);


						void write_data_response(MsgsResponsePointer  response_ptr);
			
						void read_response(const boost::system::error_code& error);

            // Scan
            void handle_read_scan(const boost::system::error_code& error);

            typename scan_connection::MsgsResponsePointer
            prepare_response_scan(MsgsRequestPointer  msg_request);

            void handle_request_scan(MsgsRequestPointer msgs_request);


            void start_read_header();


            void start_read_body(unsigned msgs_length);


            MsgsResponsePointer prepare_response(MsgsRequestPointer req);


            MsgsRequestPointer request_ptr;

            //deploy engine.
            bool deploy_scan_engine(std::vector<struct utils::meta_sig *>
                    *meta_sig_vec, std::string shm_sig_name);

            //internal message scanning.
            std::vector<struct utils::meta_sig *>  meta_sig_vec;

            policy::scan_file_controller   *scan_file_col;

            std::map<utils::filetype_code, boost::any> file_map;
            std::map<utils::filetype_code, std::vector<const char *> > file_name_map;

    };

}


#endif /* INTERNET_SCAN_CONNECTION_HPP */
