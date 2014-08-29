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
        LOG(INFO)<<"Start server... ";

        start_read_header();
    }

    //Read header is 4 bytes. End of bytes is size of body
    void scan_connection::start_read_header()
    {
        LOG(INFO)<<"Read header... ";
        //Resize size for contains Header of packet.
        msgs_read_buffer.resize(HEADER_SIZE);
        asio::async_read(msgs_socket, asio::buffer(msgs_read_buffer),
                boost::bind(&scan_connection::handle_read_header, shared_from_this(),
                        asio::placeholders::error));
    }

    void scan_connection::handle_read_header(const boost::system::error_code& error)
    {
        LOG(INFO)<<" Read header and unpack ";

        if(!error) {
            unsigned msgs_length = msgs_packed_request_scan.decode_header(msgs_read_buffer);
            LOG(INFO)<<" Header message length : " << msgs_length;
            start_read_body(msgs_length);
        }
    }

    void scan_connection::start_read_body(unsigned msgs_length)
    {
        LOG(INFO)<<"Server : start_read_body ";
        msgs_read_buffer.resize(HEADER_SIZE + msgs_length);
        asio::mutable_buffers_1 buffer = asio::buffer(&msgs_read_buffer[HEADER_SIZE], msgs_length);
        asio::async_read(msgs_socket, buffer,
                boost::bind(&scan_connection::handle_read_body, shared_from_this(),
                        asio::placeholders::error));
    }


    void scan_connection::handle_read_body(const boost::system::error_code& error)
    {
        LOG(INFO)<<" Read body and unpack ";

        //Read body before send to handle_request
        if(msgs_packed_request_scan.unpack(msgs_read_buffer)) {
            MsgsRequestPointer request_ptr = msgs_packed_request_scan.get_msg();

            if(request_ptr->type() == message_scan::RequestScan::REGISTER) {
                LOG(INFO)<<"Register : UUID request from client : " << request_ptr->uuid();
								fd_scan_map<std::pair<uint64_t, file_detail_scan> >(																
            }


        } else {
            LOG(INFO)<<" Cannot unpack message from client ";
        }
    }

}
