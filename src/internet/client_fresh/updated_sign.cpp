#include "boost/array.hpp"
#include "boost/bind.hpp"
#include "internet/client_fresh/updated_sign.hpp"
#include "internet/logger/logging.hpp"

using namespace internet;

#define FILE_BUFFER 8192

// Initial repository
// http://stackoverflow.com/questions/15274077/arguments-of-boosts-tcp-basic-resolver-query-constructor
void updated_sign::repos_initial(std::string ip_addr, std::string port)
{
    resolver::query query_dns(ip_addr.c_str(), port.c_str());
    resolver  resolver_dns(io_service);
    resolver::iterator iter_endpoint = resolver_dns.resolve(query_dns);
    protocol::endpoint endpoint = *iter_endpoint;
    error_code error;

    LOG(INFO) << "----------------- Reslove Handler --------------";
    LOG(INFO) << "IP address : " << endpoint.address().to_string();
    LOG(INFO) << "Port       : " << endpoint.port();
    LOG(INFO) << "------------------------------------------------";

    socket socket(io_service);
    socket.open(asio::ip::tcp::v4());

    socket.async_connect(endpoint, boost::bind(&updated_sign::connect_handler,
            this,
            asio::placeholders::error,
            &socket));

    io_service.run();
}



// Connected Headler
void updated_sign::connect_handler(const error_code& error, socket *sock)
{
    try {
        snprintf(uastr, sizeof(uastr),
                "ClamAV/%s (OS: linux-gnu, ARCH: x86_64, CPU: x86_64)",
                get_version());

        snprintf(cmd, sizeof(cmd),
                "GET %s/%s HTTP/1.0\r\n"
                "Host: %s\r\n%s"
                "User-Agent: %s\r\n"
                "Cache-Control: no-cache\r\n"
                "Connection: close\r\n"
                "\r\n",
                "",
                srcfile,
                remotename,
                "",
                uastr);

        LOG(INFO)<<"Write Command to server : " << cmd;
        //Write data to server database.
        sock->write_some(asio::buffer(cmd));


        //Get header data before write buffer before write to file.
        char buffer_recv[FILE_BUFFER];
        std::vector<char>   vec_buffer;
        std::size_t reply_length;
        unsigned int count_index = 0;
        char *ch;

        if((reply_length = asio::read(*sock, asio::buffer(buffer_recv, FILE_BUFFER))) > 0) {
            ch = buffer_recv;

            while(1) {
                if( count_index > 2 && *ch == '\n' &&
                        *(ch - 1) == '\r' &&
                        *(ch - 2) == '\n' &&
                        *(ch - 3) == '\r') {
                    LOG(INFO) <<" End index of header : " << count_index;
                    count_index++;

                    for(int index = count_index; index < FILE_BUFFER; index++) {
                        //Write start detail ClamAV-VDB to buffer of vector
                        vec_buffer.push_back(ch[index]);
                    }
                    break;
                }

                count_index++;
                ch++;
            }

            LOG(INFO)<<"------------- Header ------------";
            LOG(INFO)<<buffer_recv;
            LOG(INFO)<<"---------------------------------";

            //TODO: Header check error before next steps.
        }

        //Get all body of header write to file.
        while ( (reply_length = asio::read(*sock, asio::buffer(buffer_recv, FILE_BUFFER))) > 0) {

						char * end = buffer_recv + FILE_BUFFER; //Fix size of buffer. Strlen(buffer_recv) not use.
						vec_buffer.insert(vec_buffer.end(), buffer_recv, end);
         
						/* Check Start with VDB Section.
            std::string  buffer_str(buffer_recv);
            std::size_t found_vdb_index = buffer_str.find("ClamAV-VDB:");
            LOG(INFO)<<" Fond vdb index : " << found_vdb_index;
						*/
        }


		LOG(INFO)<<" Summary database size : " << vec_buffer.size();
    } catch(boost::system::system_error& error) {

        LOG(INFO) <<" Connection error : " << error.code();

    }
}
