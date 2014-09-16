#include "utils/base/common.hpp"

#include "internet/scan_server/scan_connection.hpp"

namespace internet
{

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
    }


    void scan_connection::start_read_header(const boost::system::error_code& error, std::size_t bytes)
    {
        try {
            LOG(INFO)<<"Server : start_read_header(error)";

            msgs_read_buffer.resize(HEADER_SIZE);
            msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
                    boost::bind(&scan_connection::handle_read_header, shared_from_this(),
                            _1, _2));
        } catch(boost::system::system_error& error) {
            LOG(INFO)<<"Server : start_read_header error : " << error.code();
        }
    }

    //_________________________ Scan ______________________________
    typename scan_connection::MsgsResponsePointer scan_connection::
    prepare_response_scan(MsgsRequestPointer  msg_request)
    {
        LOG(INFO)<<"Server : prepare_response_scan";
        MsgsResponsePointer  scan_response(new message_scan::ResponseScan);
        scan_response->set_uuid(msg_request->uuid());
        scan_response->set_type(message_scan::ResponseScan::SCAN_SUCCESS);
        scan_response->set_timestamp(std::string("0:0:0:0"));
        return scan_response;

    }

    void scan_connection::handle_read_scan(const boost::system::error_code& error)
    {
        LOG(INFO)<<"Server : handle_read_scan, Start read header, send to handle_read_header ";

        msgs_read_buffer.resize(HEADER_SIZE);
        msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
                boost::bind(&scan_connection::handle_read_header, shared_from_this(), //this
                        _1, _2));


    }


    void scan_connection::handle_request_scan(MsgsRequestPointer msgs_request)
    {
        LOG(INFO)<<"Server : handle_request_scan";

        try {

            //write data back to client.
            MsgsResponsePointer response_ptr =
                    prepare_response_scan(msgs_request);
            std::vector<uint8_t> write_buffer;
            packedmessage_scan<message_scan::ResponseScan> resp_msg(response_ptr);
            resp_msg.pack(write_buffer);

            msgs_socket.async_write_some(
                    asio::buffer(write_buffer),
                    boost::bind(&scan_connection::handle_read_scan,
                            shared_from_this(),
                            asio::placeholders::error));

        } catch(boost::system::system_error& error) {
            LOG(INFO)<<"Server : handle_request_scan , error : "<<error.code();
        }
    }


    //___________________________ Register ________________________
    typename scan_connection::MsgsResponsePointer scan_connection::
    prepare_response_register()
    {
        LOG(INFO)<<"Server : prepare_response_register";

        //[-]Verify UUID
        //[-]Sucess to verfity not repeatedly UUID in scanning system.
        MsgsResponsePointer  register_response(new message_scan::ResponseScan);
        register_response->set_uuid(std::string("b8000000-0000-0000-5019-f10000000000"));
        register_response->set_type(message_scan::ResponseScan::SCAN_SUCCESS);
        register_response->set_timestamp(std::string("0:0:0:0"));
        return register_response;
    }

    void scan_connection::handle_read_register(const boost::system::error_code& error)
    {
        LOG(INFO)<<"Server : handle_read_register, Start read header, send to start_read_header";
        /*
        msgs_read_buffer.resize(HEADER_SIZE);
        msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
        boost::bind(&scan_connection::start_read_header, shared_from_this(),
                _1, _2));

        */
        //start_read_header();
    }

    void scan_connection::handle_request_detail_register(const boost::system::error_code& error)
    {

        try {

            //write data back to client.
            MsgsResponsePointer response_ptr =
                    prepare_response_register();

            std::vector<uint8_t> write_buffer;
            packedmessage_scan<message_scan::ResponseScan> resp_msg(response_ptr);
            resp_msg.pack(write_buffer);

            msgs_socket.async_write_some(
                    asio::buffer(write_buffer),
                    boost::bind(&scan_connection::handle_read_register,
                            shared_from_this(),
                            asio::placeholders::error));

            LOG(INFO)<<"Server : handle_request_detail_register, write response to client completed 2sd.";

        } catch(boost::system::system_error& error) {
            LOG(INFO)<<"server : handle_request_detail_register , error : "<<error.code();
        }

    }

    void scan_connection::handle_request_register(MsgsRequestPointer msgs_request)
    {

        try {

            //write data back to client.
            MsgsResponsePointer response_ptr =
                    prepare_response_register();

            std::vector<uint8_t> write_buffer;
            packedmessage_scan<message_scan::ResponseScan> resp_msg(response_ptr);
            resp_msg.pack(write_buffer);

            msgs_socket.async_write_some(
                    asio::buffer(write_buffer),
                    boost::bind(&scan_connection::handle_request_detail_register,
                            shared_from_this(),
                            asio::placeholders::error));

            LOG(INFO)<<"Server : handle_request_register, write response to client completed 1st.";

        } catch(boost::system::system_error& error) {
            LOG(INFO)<<"server : handle_request_register , error : "<<error.code();
        }
    }

    //_________________________  Read Message request from server________________
    void scan_connection::handle_read_body(const boost::system::error_code& error)
    {
        LOG(INFO)<<"Server : handle_read_body  ";

        //Read body before send to handle_request
        if(msgs_packed_request_scan.unpack(msgs_read_buffer)) {
            request_ptr = msgs_packed_request_scan.get_msg();

            LOG(INFO)<<"Server : Message type : "<<  request_ptr->type();

            //Register UUID and IP address
            if(request_ptr->type() == message_scan::RequestScan::REGISTER) {
                LOG(INFO)<<"Server : Register type, UUID request from client : " <<
                        request_ptr->uuid();

                //[x]Loop check file type : PE, ELF and PROCESS.
                //[-]Certificate verify client connect to server.
                //[-]Timer per client connection to server.
                //[-]Insert data to structure of type : such PE, ELF
                //[-]Set message type internal or external.utils::external_msg;
                //[-]Initial SHM per files scanning PE, ELF and PROCESS.
                //[-]Initial threads supported per type such : PE, ELF and PROCESS.
                //[-]Scanning after set SHM and Thread for running.
                //TODO : Plan:-00004 supports find IP address register in system.

                //handle_request_scan(request_ptr);
                for(int count_msg = 0;
                        count_msg < request_ptr->request_set_binary_value_size();
                        count_msg++) {

                    const message_scan::RequestScan::RequestSetBinaryValue& msg_scan =
                            request_ptr->request_set_binary_value(count_msg);

                    switch(msg_scan.file_type()) {

                    case message_scan::RequestScan::PE :
                        LOG(INFO)<<"Server : Message PE scanning file name :  "<<
                                msg_scan.file_name();
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

                handle_request_register(request_ptr);

                //[-] Create handle reponse scan from client.
            }// If scanning type
        } else {
            LOG(INFO)<<" Server : Cannot unpack message from client ";
        }
    }//scan_connection::handle_read_body

    void scan_connection::handle_read_header(
            const boost::system::error_code& error,
            std::size_t bytes)
    {
        LOG(INFO)<<"Server : handle_read_header , Start read header and unpack ";
        LOG(INFO)<<"Server : handle_read_header , byte sizes : "<< bytes;

        try {
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


    }//start_read_body


}
