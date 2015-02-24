#define FOUND_INFECTED 0

#include <sstream>

#include "utils/base/common.hpp"

#include "internet/scan_server/scan_connection.hpp"

#include "internet/security/aes_controller.hpp"

namespace internet
{
    struct aes_cbc;

    using parser::clamav_sig;
    using parser::hdb_sig;

    //____________________________ Scan_connection __________________________________
    //asio::ip::tcp::socket&
    typename scan_connection::ssl_socket::lowest_layer_type&
    scan_connection::get_socket()
    {
        return msgs_socket.lowest_layer();
    }

    void scan_connection::start()
    {

        LOG(INFO)<<"Server : Start server... ";

        //start_read_header() : In case not initial with SSL;
        //start_ssl_handshake();
        LOG(INFO)<<"Server : Start() : SSL handshake";
        msgs_socket.async_handshake(boost::asio::ssl::stream_base::server,
                boost::bind(&scan_connection::handle_handshake, shared_from_this(),
                        boost::asio::placeholders::error));

    }

    //Read header is 4 bytes. End of bytes is size of body
    void scan_connection::start_read_header()
    {

        LOG(INFO)<<"Server : start_read_header.";

        //Resize size for contains Header of packet.
        msgs_read_buffer.resize(HEADER_SIZE);
        msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
                boost::bind(&scan_connection::handle_read_header, shared_from_this(),
                        _1, _2));

        //Reset timer after read data completed.
        refresh_socket_timer();
    }

    void scan_connection::
    start_socket_timer()
    {
        timer_.async_wait(boost::bind(&scan_connection::socket_timeout,
                this,
                boost::asio::placeholders::error));
    }

    bool scan_connection::
    socket_timeout(const boost::system::error_code& error)
    {
        if(error != boost::asio::error::operation_aborted) {
            msgs_socket.lowest_layer().close();
            return true;
        }
    }

    bool scan_connection::
    refresh_socket_timer()
    {
        if(msgs_socket.lowest_layer().is_open()) {
            timer_.expires_from_now(boost::posix_time::seconds(TCP_SOCKET_TIMEOUT));
            timer_.async_wait(boost::bind(&scan_connection::socket_timeout,
                    this,
                    boost::asio::placeholders::error));
        }

        return false;
    }


    //_________________________ Close socket _____________________
    //Force close socket.
    typename scan_connection::MsgsResponsePointer scan_connection::
    prepare_response_close(MsgsRequestPointer msg_request)
    {
        //[-] Add Security Module handle received/close socket from client.
        MsgsResponsePointer close_response(new message_scan::ResponseScan);
        close_response->set_uuid(msg_request->uuid());
        close_response->set_type(message_scan::ResponseScan::CLOSE_CONNECTION);
        close_response->set_timestamp(std::string("0:0:0:0"));
        //Set close socket at here.
        LOG(INFO)<"Server : prepare_reponse_close success";
        return close_response;
    }



    //_________________________ Scan ______________________________
    typename scan_connection::MsgsResponsePointer scan_connection::
    prepare_response_scan(MsgsRequestPointer  msg_request)
    {
        MsgsResponsePointer  scan_response(new message_scan::ResponseScan);
        scan_response->set_uuid(msg_request->uuid());
        scan_response->set_ip(msg_request->ip());
        scan_response->set_type(message_scan::ResponseScan::SCAN_SUCCESS);
        scan_response->set_timestamp(std::string("0:0:0:0"));
        //security reason
        scan_response->set_conn_ip(msg_request->ip());
        scan_response->set_conn_uuid(msg_request->uuid());
        LOG(INFO)<<"Server : prepare_response_scan success";
        return scan_response;

    }


    //___________________________ Register ________________________
    typename scan_connection::MsgsResponsePointer scan_connection::
    prepare_response_register(MsgsRequestPointer  msgs_request)
    {

        //[-]Verify UUID(UUID Controller)
        //[-]Sucess to verfity not repeatedly UUID in scanning system.
        //[-]Add Security Module handle received/close socket from client.
        MsgsResponsePointer  register_response(new message_scan::ResponseScan);
        register_response->set_uuid(msgs_request->uuid());
        register_response->set_type(message_scan::ResponseScan::REGISTER_SUCCESS);
        register_response->set_timestamp(std::string("0:0:0:0"));

        LOG(INFO)<<"Server : prepare_response_register success";

        return register_response;
    }


    void scan_connection::write_response(MsgsRequestPointer  request_ptr, MsgsResponsePointer response_ptr)
    {
        LOG(INFO)<<"------------------------Write Response--------------------------";

        try {

            LOG(INFO)<<"Server :  write_response, Client request msgs type : "<<request_ptr->type();

            std::vector<uint8_t> write_buffer;
            packedmessage_scan<message_scan::ResponseScan> resp_msg(response_ptr);
            resp_msg.pack(write_buffer);

            msgs_socket.async_write_some(
                    asio::buffer(write_buffer),
                    boost::bind(&scan_connection::start_read_header_sec, //after register send to sec
                            shared_from_this()));

            //step for  write to client in NON-SSL mode.

            LOG(INFO)<<"Server : write_response, write response to client completed 1st.";
            //refresh timer.
            refresh_socket_timer();

        } catch(boost::system::system_error& error) {

            LOG(INFO)<<"server : write_response, error : "<<error.code();

        }

        LOG(INFO)<<"-------------------------------------------------------------";
    }

    bool  scan_connection::handle_close_process(MsgsRequestPointer request_ptr)
    {

        std::string uuid = request_ptr->uuid();

        //UUID not value
        if(uuid.empty()) {
            //lock_.unlock();
            return false;
        }

        //Checking UUID from database.
        return true;
    }

    bool	scan_connection::handle_scan_process(MsgsRequestPointer request_ptr)
    {

        LOG(INFO)<<"Server : Register internal message size : " <<
                request_ptr->set_binary_value_size();

        utils::filetype_code file_type;

        struct MAPPED_FILE_PE *s_mapped_fpe;

        const char *file_name;

        std::vector<MAPPED_FILE_PE *>   mapped_file_vec;
        std::vector<const char *>       file_type_vec;
        uint8_t *binary_temp;

        for(int count_msg = 0;
                count_msg < request_ptr->set_binary_value_size();
                count_msg++) {

            const message_scan::RequestScan::SetBinaryValue& msg_scan =
                    request_ptr->set_binary_value(count_msg);


            switch(msg_scan.file_type()) {


            case message_scan::RequestScan::PE :

                LOG(INFO)<<"Scan Request PE type...";

                mapped_file_vec.push_back(new MAPPED_FILE_PE);

                //get last elements after push back new object.
                s_mapped_fpe = mapped_file_vec.back();
                //set external signature
                s_mapped_fpe->msg_type = utils::external_msg;
                //set File name
                s_mapped_fpe->file_name = msg_scan.file_name();

                //set Data
                //s_mapped_fpe->data
                // memcpy(s_mapped_fpe->data, msg_scan.binary().c_str(), msg_scan.binary().size());
                s_mapped_fpe->data = new uint8_t[msg_scan.binary().size()+1];
                memcpy(s_mapped_fpe->data, (uint8_t *)msg_scan.binary().c_str(), msg_scan.binary().size());
                //s_mapped_fpe->data[msg_scan.binary().size()] = '\0';
                //set size of data
                s_mapped_fpe->size = msg_scan.binary().size();


                //Pe type finds in file_name map. std::map allocated before find.
                //We use reference to value and change its in s_mapped_fpe.
                //fname_vec = &file_name_map.find(utils::pe_file)->second;

                //File Name
                //Initial first value.
                file_type_vec.push_back(std::string("").c_str());
                //Get reference of last file name initialed.
                file_name = file_type_vec.back();
                //Set file name
                file_name = msg_scan.file_name().c_str();

                LOG(INFO)<<"Server : Message PE scanning file name : "<<
                        msg_scan.file_name();
                LOG(INFO)<<"Binary data    : "<< msg_scan.binary();
                LOG(INFO)<<"File  type     : "<< msg_scan.file_type();
                LOG(INFO)<<"Binary data compare : " << s_mapped_fpe->data;

                break;

            case message_scan::RequestScan::ELF :
                LOG(INFO)<<"Server : Message ELF scanning file name : "<<
                        msg_scan.file_name();
                break;

            case message_scan::RequestScan::PROCESS :
                LOG(INFO)<<"Server : Message PROCESS scanning file name : "<<
                        msg_scan.file_name();
                break;

            default:
                LOG(INFO)<<"Server : Not type scanning support file name : "<<
                        msg_scan.file_name();
                break;

            }//switch

        }//for


        //Scanning virus.
        scan_file_->set_file(&mapped_file_vec, &file_type_vec);

        // Find Engine for file type.
        if(scan_file_->find_engine(utils::pe_file)) {
            LOG(INFO)<<"Server : Find PE-Engine scanning after register success!";
        }

        // Scan file
        threatinfo_vec_type threatinfo_vec = scan_file_->scan_file();

        threatinfo_vec_type::iterator iter_threatinfo_vec;
			
        for(iter_threatinfo_vec = threatinfo_vec.begin();
                iter_threatinfo_vec != threatinfo_vec.end();
                ++iter_threatinfo_vec) {
            threatinfo_type *threat_info = *iter_threatinfo_vec;
						
            if(threat_info->file_name().size() == 0) continue;

            //Loop insert result to message
            for(int count_result = 0;
                    count_result < request_ptr->set_binary_value_size();
                    count_result++) {
								
                message_scan::RequestScan::SetBinaryValue * msg_scan_result =
                        request_ptr->mutable_set_binary_value(count_result);

								//LOG(INFO)<<"File name : " << msg_scan_result->file_name();

                if(threat_info->file_name().compare(msg_scan_result->file_name()) == FOUND_INFECTED) {

                    LOG(INFO)<<"Server scan file name : " << threat_info->file_name() <<", Success! ";
                    LOG(INFO)<<"File name from client : " << msg_scan_result->file_name();
                    msg_scan_result->set_file_status(message_scan::RequestScan::FILE_INFECTED); 
                }else{
                    msg_scan_result->set_file_status(message_scan::RequestScan::FILE_CLEAN);
								}
								
            }//for

        }//for

        return true;
    }

    //_________________________  Read Message request from server________________
    //[x]Loop check file type : PE, ELF and PROCESS.
    //[-]Certificate verify client connect to server(Certificate Controller).
    //[x]Timer per client connection to server.
    //[x-PE,-]Insert data to structure of type : such PE, ELF
    //[x]Set message type internal or external.utils::external_msg;
    //[x]Initial SHM per files scanning PE, ELF and PROCESS.
    //[x]Initial threads supported per type such : PE, ELF and PROCESS.
    //[x]Scanning after set SHM and Thread for running.
    //TODO : Plan:-00004 supports find IP address register in system.

    void scan_connection::handle_read_body(const boost::system::error_code& error)
    {
        LOG(INFO)<<"Server : handle_read_body  ";



        //Read body before send to handle_request
        if(msgs_packed_request_scan.unpack(msgs_read_buffer)) {
            request_ptr = msgs_packed_request_scan.get_msg();

            LOG(INFO)<<"------------------Read Body-----------------------------";

            LOG(INFO)<<"Server : Register type, UUID request from client : " << request_ptr->uuid();
            LOG(INFO)<<"Server : Message process type : " << request_ptr->type();

            std::string ip_addr;
            std::string uuid;

            internet::security::aes_cbc *aes;
            internet::security::aes_cbc *aes_external;

            switch(request_ptr->type()) {

            case message_scan::RequestScan::REGISTER :

                ip_addr =
                        msgs_socket.lowest_layer().remote_endpoint().address().to_string();


                uuid = request_ptr->uuid();

                aes_external =
                        new internet::security::aes_cbc(ip_addr.c_str(), uuid.c_str());
                //Encryption

                LOG(INFO)<<"Initial key & iv support server-sec, UUID : "<< uuid <<", IP : "<<ip_addr;

                //internet::security::aes_cbc * aes;
                aes = enc_controller_->process_crypto(*aes_external, utils::insert_key_crypto_mode);

                LOG(INFO)<<"Key : " << aes->key;
                LOG(INFO)<<"IV  : " << aes->iv;

                request_ptr->set_key((const char *)aes->key);

                request_ptr->set_iv((const char *)aes->iv);

                LOG(INFO)<<"------------------- REGISTER TYPE---------------";
                //Write data register success and send symmetric key exchanged to client.
                write_response(request_ptr, prepare_response_register_sec(request_ptr, aes));

                LOG(INFO)<<"Fall back to Symmetric encryption key";
                //Write 3DES message to client.That key exchanged success.
                SSL_set_cipher_list(msgs_socket.native_handle(), "eNULL");
                SSL_set_options(msgs_socket.native_handle(), SSL_OP_NO_COMPRESSION);

                LOG(INFO)<<"----------------END REGISTER TYPE---------------";

                break;

            default :
                LOG(INFO)<<"------------------ No client type support ---------------";
                break;

            }//end switch.

        } else {
            LOG(INFO)<<" Server : Cannot unpack message from client ";
        }//if-else unpack
    }//scan_connection::handle_read_body

    void scan_connection::handle_read_header(
            const boost::system::error_code& error,
            std::size_t bytes)
    {
        LOG(INFO)<<"Server : handle_read_header , Start read header and unpack ";

        try {
            LOG(INFO)<<"Server Hex header :" << show_hex(msgs_read_buffer);
            unsigned msgs_length =
                    msgs_packed_request_scan.decode_header(msgs_read_buffer);
            LOG(INFO)<<" Header message length : " << msgs_length;
            start_read_body(msgs_length);
        } catch(boost::system::system_error e) {
            LOG(INFO)<<"handle_read_header, error : " << e.code();
        }
    }

    void scan_connection::start_read_body(unsigned msgs_length)
    {
        LOG(INFO)<<"Server : start_read_body ";
        msgs_read_buffer.resize(HEADER_SIZE + msgs_length);
        asio::mutable_buffers_1 buffer =
                asio::buffer(&msgs_read_buffer[HEADER_SIZE], msgs_length);
        asio::async_read(msgs_socket, buffer,
                boost::bind(&scan_connection::handle_read_body, shared_from_this(),
                        asio::placeholders::error));
        //Refresh socket
        refresh_socket_timer();
    }//start_read_body

    //Frist step, Start socket SSL.
    void scan_connection::start_ssl_handshake()
    {
        LOG(INFO)<<"Server : Start_ssl_handshake() : SSL handshake";
        msgs_socket.async_handshake(boost::asio::ssl::stream_base::server,
                boost::bind(&scan_connection::handle_handshake, this,
                        boost::asio::placeholders::error));
    }

    void scan_connection::handle_handshake(const boost::system::error_code& error)
    {
        LOG(INFO)<<"Server : SSL Handle handshake";

        if(!error) {
            //Resize size for contains Header of packet.
            msgs_read_buffer.resize(HEADER_SIZE);
            msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
                    boost::bind(&scan_connection::handle_read_header, shared_from_this(),
                            _1, _2));

            //Reset timer after read data completed.
            refresh_socket_timer();

        } else {
            LOG(INFO)<<" Server : error  : " <<  error.message();
        }
    }

    //___________________________________Step SSL & AES fallback
    typename scan_connection::MsgsResponsePointer scan_connection::
    prepare_response_register_sec(MsgsRequestPointer  msgs_request,
            internet::security::aes_cbc   *aes)
    {

        //[-]Verify UUID(UUID Controller)
        //[-]Sucess to verfity not repeatedly UUID in scanning system.
        //[x]Add Security Module handle received/close socket from client.
        MsgsResponsePointer  register_response(new message_scan::ResponseScan);

        register_response->set_uuid(msgs_request->uuid());
        register_response->set_type(message_scan::ResponseScan::REGISTER_SUCCESS);
        register_response->set_timestamp(std::string("0:0:0:0"));

        register_response->set_key(msgs_request->key());
        register_response->set_iv(msgs_request->iv());

        register_response->set_ip(msgs_request->ip());
        register_response->set_conn_ip(msgs_request->conn_ip());
        register_response->set_conn_uuid(msgs_request->conn_uuid());

        LOG(INFO)<<"Server : prepare_response_register success";

        return register_response;
    }//prepare_response_register_sec

    //Read header is 4 bytes. End of bytes is size of body
    void scan_connection::start_read_header_sec()
    {

        LOG(INFO)<<"Server : start_read_header_sec.";

        //Resize size for contains Header of packet.
        msgs_read_buffer.resize(HEADER_SIZE);
        msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
                boost::bind(&scan_connection::handle_read_header_sec, shared_from_this(),
                        _1, _2));

        //Reset timer after read data completed.
        refresh_socket_timer();
    }//start_read_header_sec

    //call read body after header calculated.
    void scan_connection::start_read_body_sec(unsigned msgs_length)
    {
        LOG(INFO)<<"Server : start_read_body_sec";
        msgs_read_buffer.resize(HEADER_SIZE + msgs_length);
        asio::mutable_buffers_1 buffer =
                asio::buffer(&msgs_read_buffer[HEADER_SIZE], msgs_length);
        asio::async_read(msgs_socket, buffer,
                boost::bind(&scan_connection::handle_read_body_sec, shared_from_this(),
                        asio::placeholders::error));
        //Refresh socket
        refresh_socket_timer();
    }//start_read_body_sec

    void scan_connection::handle_read_header_sec(
            const boost::system::error_code& error,
            std::size_t bytes)
    {
        LOG(INFO)<<"Server : handle_read_header_sec , Start read header and unpack ";

        try {
            LOG(INFO)<<"Server Hex header :" << show_hex(msgs_read_buffer);
            unsigned msgs_length =
                    msgs_packed_request_scan.decode_header(msgs_read_buffer);
            LOG(INFO)<<" Header message length : " << msgs_length;
            start_read_body_sec(msgs_length);
        } catch(boost::system::system_error e) {
            LOG(INFO)<<"handle_read_header, error : " << e.code();
        }
    }//handle_read_header_sec

    void scan_connection::handle_read_body_sec(const boost::system::error_code& error)
    {
        LOG(INFO)<<"Server : handle_read_body  ";

        //Read body before send to handle_request
        if(msgs_packed_request_scan.unpack(msgs_read_buffer)) {
            request_ptr = msgs_packed_request_scan.get_msg();

            LOG(INFO)<<"------------------Read Body-----------------------------";

            //decrypt message.
            secure_field_req->decryption(request_ptr, enc_controller_);

            LOG(INFO)<<"Server : Register type, UUID request from client : " << request_ptr->uuid();
            LOG(INFO)<<"Server : Message process type : " << request_ptr->type();


            switch(request_ptr->type()) {

            case message_scan::RequestScan::SCAN :

                LOG(INFO)<<"---------------------SCAN TYPE--------------------";

                if(!handle_scan_process(request_ptr)) {
                    LOG(INFO)<<" Cannot scanning message from client.";
                }

                LOG(INFO)<<"------------------END SCAN TYPE-------------------";

                write_response_sec(request_ptr, prepare_response_scan(request_ptr));

                break;

            case message_scan::RequestScan::CLOSE_CONNECTION :
                //Close socket in thread.
                LOG(INFO)<<"-------------------- CLOSE Connection --------------";

                if(!handle_close_process(request_ptr)) {
                    LOG(INFO)<<" Cannot close message internal process problem";
                }

                write_response_sec(request_ptr, prepare_response_close(request_ptr));

                if(msgs_socket.lowest_layer().is_open()) {
                    msgs_socket.lowest_layer().close();
                    LOG(INFO)<<"-------------------- CLOSE Connection From Server compeleted -------------";
                }

                break;

            default :
                LOG(INFO)<<"---------------- No type of client support -------------";
                break;
            }// switch

        } else {
            LOG(INFO)<<" Server : Cannot unpack message from client ";
        }//if-else unpack
    }//scan_connection::handle_read_body_sec

    void scan_connection::
    write_response_sec(MsgsRequestPointer  request_ptr, MsgsResponsePointer response_ptr)
    {
        LOG(INFO)<<"------------------------Write Response--------------------------";

        try {

            LOG(INFO)<<"Server :  write_response_sec, Client request msgs type : "<<request_ptr->type();

            LOG(INFO)<<"Server :  write_response_sec, Message Encrypted before send to network";

            secure_field_resp->encryption(response_ptr, enc_controller_);

            std::vector<uint8_t> write_buffer;
            packedmessage_scan<message_scan::ResponseScan> resp_msg(response_ptr);

            resp_msg.pack(write_buffer);

            msgs_socket.async_write_some(
                    asio::buffer(write_buffer),
                    boost::bind(&scan_connection::start_read_header_sec,
                            shared_from_this()));

            LOG(INFO)<<"Server : write_response_sec, write response to client completed 1st.";
            //refresh timer.
            refresh_socket_timer();

        } catch(boost::system::system_error& error) {

            LOG(INFO)<<"server : write_response_sec, error : "<<error.code();

        }

        LOG(INFO)<<"-------------------------------------------------------------";
    }//write_response_sec


}//network
