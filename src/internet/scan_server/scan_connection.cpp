#include "utils/base/common.hpp"

#include "internet/scan_server/scan_connection.hpp"

namespace internet
{

    using parser::clamav_sig;
    using parser::hdb_sig;

    //____________________________ Scan_connection __________________________________
    asio::ip::tcp::socket& scan_connection::get_socket()
    {
        return msgs_socket;
    }

    void scan_connection::start()
    {

        LOG(INFO)<<"Server : Start server... ";

        start_read_header();
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
            msgs_socket.close();
            return true;
        }
    }

    bool scan_connection::
    refresh_socket_timer()
    {
        if(msgs_socket.is_open()) {
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
        scan_response->set_type(message_scan::ResponseScan::SCAN_SUCCESS);
        scan_response->set_timestamp(std::string("0:0:0:0"));
        //scan_response->set_file_name(msg_request->file_name());
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
                    boost::bind(&scan_connection::start_read_header,
                            shared_from_this()));

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
        if(uuid.empty()){
						//lock_.unlock();
            return false;
				}
        //Checking UUID from database.
        return true;
    }

    bool	scan_connection::handle_scan_process(MsgsRequestPointer request_ptr)
    {

        LOG(INFO)<<"Server : Register internal message size : " <<
                request_ptr->request_set_binary_value_size();

        utils::filetype_code file_type;

        struct MAPPED_FILE_PE *s_mapped_fpe;

        const char *file_name;

        std::vector<MAPPED_FILE_PE *>   mapped_file_vec;
        std::vector<const char *>       file_type_vec;


        for(int count_msg = 0;
                count_msg < request_ptr->request_set_binary_value_size();
                count_msg++) {

            const message_scan::RequestScan::RequestSetBinaryValue& msg_scan =
                    request_ptr->request_set_binary_value(count_msg);


            switch(msg_scan.file_type()) {


            case message_scan::RequestScan::PE :


                mapped_file_vec.push_back(new MAPPED_FILE_PE);

                //get last elements after push back new object.
                s_mapped_fpe = mapped_file_vec.back();
                //set external signature
                s_mapped_fpe->msg_type = utils::external_msg;
                //set File name
                s_mapped_fpe->file_name = msg_scan.file_name();
                //set Data
                s_mapped_fpe->data =  (uint8_t *)(msg_scan.binary().c_str());
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
            LOG(INFO)<<"Server scan file name : " << threat_info->file_name() <<", Success! ";
        }
        return true;
    }

    //_________________________  Read Message request from server________________
    //[x]Loop check file type : PE, ELF and PROCESS.
    //[-]Certificate verify client connect to server(Certificate Controller).
    //[-]Timer per client connection to server.
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

            std::vector<MAPPED_FILE_PE *>   mapped_file_vec;
            std::vector<const char *>       file_type_vec;

            switch(request_ptr->type()) {

            case message_scan::RequestScan::REGISTER :

                LOG(INFO)<<"------------------- REGISTER TYPE---------------";

                write_response(request_ptr, prepare_response_register(request_ptr));

                LOG(INFO)<<"----------------END REGISTER TYPE---------------";

                break;

            case message_scan::RequestScan::SCAN :

                LOG(INFO)<<"---------------------SCAN TYPE--------------------";

                if(!handle_scan_process(request_ptr)) {
                    LOG(INFO)<<" Cannot scanning message from client.";
                }

                LOG(INFO)<<"------------------END SCAN TYPE-------------------";

                write_response(request_ptr, prepare_response_scan(request_ptr));

                break;

            case message_scan::RequestScan::CLOSE_CONNECTION :
                //Close socket in thread.
                LOG(INFO)<<"-------------------- CLOSE Connection --------------";

                if(!handle_close_process(request_ptr)) {
                    LOG(INFO)<<" Cannot close message internal process problem";
                }

                write_response(request_ptr, prepare_response_close(request_ptr));

                if(msgs_socket.is_open()) {
                    msgs_socket.close();
                    LOG(INFO)<<"-------------------- CLOSE Connection From Server compeleted -------------";
                }

                break;

            default :
                break;
            }

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


}
