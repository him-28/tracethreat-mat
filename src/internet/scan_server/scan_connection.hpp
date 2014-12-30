#ifndef INTERNET_SCAN_CONNECTION_HPP
#define INTERNET_SCAN_CONNECTION_HPP

//Default timeout support client. We plan use configure from database configure of server service.
#define TCP_SOCKET_TIMEOUT   600000000000000000

//#define NON_ENCRYPTION_ENULL eNULL

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

#include <boost/thread.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

//#include <memory>
#include <vector>

#include "scan/scan_file_controller.hpp"

#include "internet/utils/common.hpp"

#include "internet/logger/logging.hpp"

#include "internet/msg/scan_server_client/message_scan.pb.h"
//#include "internet/scan_server/packedmessage_scan.hpp"
#include "internet/msg/packedmessage_scan.hpp"

#include "gensign/clamavsig.hpp"

#include "utils/logger/clutil_logger.hpp"

namespace internet
{
    namespace asio = boost::asio;

	  namespace h_util = hnmav_util; 

    class scan_connection	 : public boost::enable_shared_from_this<scan_connection>
    {

        public:
            typedef boost::shared_ptr<scan_connection> pointer;
            typedef boost::shared_ptr<message_scan::RequestScan>  MsgsRequestPointer;
            typedef boost::shared_ptr<message_scan::ResponseScan> MsgsResponsePointer;

						typedef policy::scan_internet_controller<struct MAPPED_FILE_PE>  scan_file_type;

						typedef message_tracethreat::InfectedFileInfo  threatinfo_type;
		
						typedef std::vector<threatinfo_type*>  threatinfo_vec_type;

						typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

						//typedef boost::shared_ptr<asio::io_service> & io_service_type;
	
						typedef asio::io_service & io_service_type;

            static pointer create(io_service_type io_service, 
																  scan_file_type * scan_file,
																	asio::ssl::context & context) {
							  
                return scan_connection::pointer(new scan_connection(io_service, scan_file, context));

            }

            //asio::ip::tcp::socket& 
						ssl_socket::lowest_layer_type & get_socket();

            void start();

						~scan_connection(){		
							
						}

        private:
						//Set default is true. First step for SSL connection and fallback to snappy+3DES	
						bool sslEnable;

						asio::ssl::stream<asio::ip::tcp::socket> msgs_socket;
            //asio::ip::tcp::socket msgs_socket;
		
						//asio::ssl::context context_;

            std::vector<uint8_t> msgs_read_buffer;

            packedmessage_scan<message_scan::RequestScan> msgs_packed_request_scan;

            std::map<std::string, file_detail_scan> fd_scan_map;

            scan_connection(io_service_type io_service,  //asio::io_service&
														scan_file_type * scan_file,
														asio::ssl::context & context) :
                msgs_packed_request_scan(boost::shared_ptr<message_scan::RequestScan>(
                        new message_scan::RequestScan())),
								scan_file_(scan_file),
								timer_(io_service, boost::posix_time::seconds(TCP_SOCKET_TIMEOUT)),
                msgs_socket(io_service,  context)
                {
								LOG(INFO)<<" Scan_connection : start timer";
								//Start timer check timeout per connection.
								start_socket_timer(); 
            }

						//Frist response to client.
					  typename scan_connection::MsgsResponsePointer 
						prepare_response(MsgsRequestPointer req);

						//Register step. 
            typename scan_connection::MsgsResponsePointer
            prepare_response_register(MsgsRequestPointer);

						//Scanning steps
           typename scan_connection::MsgsResponsePointer
            prepare_response_scan(MsgsRequestPointer  msg_request);

						typename scan_connection::MsgsResponsePointer
						prepare_response_close(MsgsRequestPointer msg_request);

						//Handle scan engine.
				    bool handle_scan_process(MsgsRequestPointer request_ptr);

						//Handle 
						bool handle_close_process(MsgsRequestPointer request_ptr);

            //Response
            void write_response(MsgsRequestPointer  request_ptr, MsgsResponsePointer  response_ptr);
 
            void start_read_header();

            void start_read_body(unsigned msgs_length);

            void handle_read_header(const boost::system::error_code& error, std::size_t bytes);

            void handle_read_body(const boost::system::error_code& error);

						//utils handler socket timeout connection 
						//:"Boost asio::deadline_timer is resetting before timeout " http://goo.gl/ACSd19
						bool refresh_socket_timer();
		
						//socket close
						bool socket_timeout(const boost::system::error_code & error);
						
						//socket timeout initial 
						void start_socket_timer();
	
						boost::asio::deadline_timer  timer_;

						void start_ssl_handshake();

						void handle_handshake(const boost::system::error_code& error);

						//Message request from server. 
            MsgsRequestPointer request_ptr;

						//Signature database path.
						std::string file_sig_path_;

						parser::sig_parser * sig_parse;

					  scan_file_type * scan_file_;

						 
    };

}


#endif /* INTERNET_SCAN_CONNECTION_HPP */
