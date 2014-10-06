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

            typedef boost::shared_ptr<utils::meta_sig>  msig_ptr;

            static pointer create(asio::io_service& io_service, std::string file_sig_path) {
							  
                return scan_connection::pointer(new scan_connection(io_service, file_sig_path));

            }//, message_scan& msgs);

            asio::ip::tcp::socket& get_socket();

            void start();


        private:

            asio::ip::tcp::socket msgs_socket;
            //message_scan& msgs_scan_ref;
            std::vector<uint8_t> msgs_read_buffer;

            packedmessage_scan<message_scan::RequestScan> msgs_packed_request_scan;

            std::map<std::string, file_detail_scan> fd_scan_map;

            scan_connection(asio::io_service& io_service, std::string file_sig_path) :
                msgs_socket(io_service),
                msgs_packed_request_scan(boost::shared_ptr<message_scan::RequestScan>(
                        new message_scan::RequestScan())),
								file_sig_path_(file_sig_path) {


								//[x] Call to scanning system.
								if(deploy_scan_engine()){
										LOG(INFO)<<"Deploy scan engine completed.";
								}

                //[x] Initial data support map file detail container.
                //[x] Initial data support map file name container.
                LOG(INFO)<<"Scan connections initial type support";

								//[x] Initial internal logger
								//initial_internal_logger();
							
								//Second solution support multiple file scanning.
                //auto pos = file_map.find(utils::pe_file);
                //pos->second  = std::vector<MAPPED_FILE_PE *>();
                //file_name_map.insert(std::make_pair<utils::filetype_code, std::vector<const char *> >
                //        (utils::pe_file, std::vector<const char *>()));

                LOG(INFO)<<"PE type supported";

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

            //Response
            void write_response(MsgsRequestPointer  request_ptr, MsgsResponsePointer  response_ptr);
 
            void start_read_header();

            void start_read_body(unsigned msgs_length);

            void handle_read_header(const boost::system::error_code& error, std::size_t bytes);

            void handle_read_body(const boost::system::error_code& error);
 
            MsgsRequestPointer request_ptr;

						//Signature database path.
						std::string file_sig_path_;

						//Load scan type support scanning system.
						bool deploy_scan_system();

            //deploy engine.
            bool deploy_scan_engine();

						parser::sig_parser * sig_parse;

		 	    	std::vector<msig_ptr>  msig_ptr_vec; 

						std::vector<parser::meta_sigparse*>  * msig_parse_vec; 

            //policy::scan_file_controller   *scan_file_col;
						policy::scan_pe_internet_controller<struct MAPPED_FILE_PE> * scan_file_pe;

            //std::map<utils::filetype_code, boost::any> file_map;
            //std::map<utils::filetype_code, std::vector<const char *> > file_name_map;

    };

}


#endif /* INTERNET_SCAN_CONNECTION_HPP */
