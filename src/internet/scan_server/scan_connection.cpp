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
                    boost::bind(&scan_connection::start_read_header,//handle_read_scan
                            shared_from_this()));
            //asio::placeholders::error));

        } catch(boost::system::system_error& error) {
            LOG(INFO)<<"Server : handle_request_scan , error : "<<error.code();
        }
    }


    //___________________________ Register ________________________
    typename scan_connection::MsgsResponsePointer scan_connection::
    prepare_response_register(MsgsRequestPointer  msgs_request)
    {
        LOG(INFO)<<"Server : prepare_response_register";

        //[-]Verify UUID
        //[-]Sucess to verfity not repeatedly UUID in scanning system.
        MsgsResponsePointer  register_response(new message_scan::ResponseScan);
        register_response->set_uuid(msgs_request->uuid());
        register_response->set_type(message_scan::ResponseScan::REGISTER_SUCCESS);
        register_response->set_timestamp(std::string("0:0:0:0"));
        return register_response;
    }

    void scan_connection::read_response(const boost::system::error_code& error)
    {

        LOG(INFO)<<"Server : read_response.";

        msgs_read_buffer.resize(HEADER_SIZE);
        msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
                boost::bind(&scan_connection::handle_read_header, shared_from_this(),
                        _1, _2));

        LOG(INFO)<<"Server : read_response, read data from client";
    }

    void scan_connection::write_data_response(MsgsResponsePointer  response_ptr)
    {

        LOG(INFO)<<"------------------------Write Data Response--------------------------";

        LOG(INFO)<<"Server : write_data_response, Send response to client msgs type :"<<
                response_ptr->type();

        try {
            /*
                        std::vector<uint8_t> write_buffer;
                        packedmessage_scan<message_scan::ResponseScan> resp_msg(response_ptr);
                        resp_msg.pack(write_buffer);

                        msgs_socket.async_write_some(
                                asio::buffer(write_buffer),
                                boost::bind(&scan_connection::read_response,
                                        shared_from_this(),
                                        asio::placeholders::error));
            */

            msgs_read_buffer.resize(HEADER_SIZE);
            msgs_socket.async_read_some(asio::buffer(msgs_read_buffer),
                    boost::bind(&scan_connection::handle_read_header, shared_from_this(),
                            _1, _2));


            LOG(INFO)<<"Server : write_data_response , write data to client completed 2rd.";

        } catch(boost::system::system_error& error) {
            LOG(INFO)<<"server : write_data_response , error : "<<error.code();
        }

        LOG(INFO)<<"-----------------------------------------------------------------";


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
                    boost::bind(&scan_connection::write_data_response,
                            shared_from_this(),
                            response_ptr));

            LOG(INFO)<<"Server : write_response, write response to client completed 1st.";

        } catch(boost::system::system_error& error) {

            LOG(INFO)<<"server : write_response, error : "<<error.code();

        }

        LOG(INFO)<<"-------------------------------------------------------------";

    }

    //_________________________  Read Message request from server________________
    //[x]Loop check file type : PE, ELF and PROCESS.
    //[-]Certificate verify client connect to server.
    //[-]Timer per client connection to server.
    //[-]Insert data to structure of type : such PE, ELF
    //[-]Set message type internal or external.utils::external_msg;
    //[-]Initial SHM per files scanning PE, ELF and PROCESS.
    //[-]Initial threads supported per type such : PE, ELF and PROCESS.
    //[-]Scanning after set SHM and Thread for running.
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

            if(request_ptr->type() == message_scan::RequestScan::REGISTER) {
                LOG(INFO)<<"------------------- REGISTER TYPE---------------";
                write_response(request_ptr, prepare_response_register(request_ptr));
                LOG(INFO)<<"----------------END REGISTER TYPE---------------";
            }//if register


            //Register UUID and IP address
            if(request_ptr->type() == message_scan::RequestScan::SCAN) {

                LOG(INFO)<<"---------------------SCAN TYPE--------------------";

                LOG(INFO)<<"Server : Register internal message size : " <<
                        request_ptr->request_set_binary_value_size();

                utils::filetype_code file_type;
                struct MAPPED_FILE_PE *s_mapped_fpe;
                const char *file_name;

                for(int count_msg = 0;
                        count_msg < request_ptr->request_set_binary_value_size();
                        count_msg++) {

                    const message_scan::RequestScan::RequestSetBinaryValue& msg_scan =
                            request_ptr->request_set_binary_value(count_msg);

                    std::vector<MAPPED_FILE_PE *>   *fpe_vec;
                    std::vector<const char *>       *fname_vec;

                    switch(msg_scan.file_type()) {


                    case message_scan::RequestScan::PE :
                        LOG(INFO)<<"Server : Message PE scanning file name :  "<<
                                msg_scan.file_name();

                        //set file_type for specific file type.
                        fpe_vec = boost::any_cast<std::vector<MAPPED_FILE_PE *> >
                                (&file_map.find(utils::pe_file)->second);

                        //Pe type finds in file_name map. std::map allocated before find.
                        //We use reference to value and change its in s_mapped_fpe.
                        fname_vec = &file_name_map.find(utils::pe_file)->second;


                        //get last elements after push back new object.
                        s_mapped_fpe = fpe_vec->back();
                        //set external signature
                        s_mapped_fpe->msg_type = utils::external_msg;
                        //set File name
                        s_mapped_fpe->file_name = msg_scan.file_name();
                        //set Data
                        s_mapped_fpe->data =  (uint8_t *)(msg_scan.binary().c_str());
                        //set size of data
                        s_mapped_fpe->size = msg_scan.binary().size();

                        //File Name
                        fpe_vec->push_back(new MAPPED_FILE_PE);
                        //Initial first value.
                        fname_vec->push_back(std::string("").c_str());
                        //Get reference of last file name initialed.
                        file_name = fname_vec->back();
                        //Set file name
                        file_name = msg_scan.file_name().c_str();

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

                /*
                                //[] Scanning virus.
                                scan_file_col->set_file(mapped_file_vec, file_type_vec);
                                // Find Engine for file type.
                                scan_file_col->find_engine(file_type);
                                // Scan file
                handle_request_register(request_ptr);
                */
                //handle_request_register(request_ptr);
                LOG(INFO)<<"------------------END SCAN TYPE-------------------";

            }// If scanning type

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


    }//start_read_body


    //Deploy scanning system and load object before call scan_file member function.
    bool scan_connection::deploy_scan_engine(
            std::vector<struct utils::meta_sig *> *meta_sig_vec,
            std::string shm_sig_name)
    {

        LOG(INFO)<<"Start deploy scan engine.";

        //Load PE Scanning engine.
        LOG(INFO)<<"PE Scanning Engine start.";
        scan_file_col = new policy::scan_pe_internet_controller<struct MAPPED_FILE_PE>();

        if(scan_file_col->load_database(meta_sig_vec, shm_sig_name)) {
            LOG(INFO)<<" PE Scanning cannot load_database";
            return false;
        }

        if(scan_file_col->load_engine(utils::pe_file)) {
            LOG(INFO)<<"  PE Scanning cannot load_engine";
            return false;
        }

        LOG(INFO)<<"Deploy system success";
    }

}
