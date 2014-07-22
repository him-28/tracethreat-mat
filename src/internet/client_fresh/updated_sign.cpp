#include "boost/array.hpp"
#include "boost/bind.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

#include "internet/client_fresh/updated_sign.hpp"
#include "internet/logger/logging.hpp"

using namespace internet;

#define FILE_BUFFER 8192
#define HEADER_RECV_DB  10
#define HEADER_INTERNAL_PARSE 7
#define HTTP_CONTAIN_LENGTH 6

BOOST_FUSION_ADAPT_STRUCT(
        http_header,
        (std::string, type)
        (std::string, value)
)

bool updated_sign::parse_header_cvd(std::string const&  header_str,
        std::vector<http_header *>& header_vector,
        uint8_t count_header)
{

    //LOG(INFO)<< "Start parse header : " << header_str;

    bool r = true;

    struct http_header  header;
    http_header_type  header_type(boost::fusion::as_nview<0, 1>(header));


    rule_http_header = *(boost::spirit::qi::char_ - ':') >> ":"
            >> *(boost::spirit::qi::char_);

    iterator_type begin = header_str.begin();

    r = boost::spirit::qi::parse(begin, header_str.end(), rule_http_header, header_type);

    if(r) {
        struct http_header *http = header_vector[count_header];
        
        http->type = header.type;

        uint32_t end_line = header.value.find_first_of("\n");

        if(end_line != std::string::npos)
            header.value.erase(end_line, header.value.size() - end_line);

        http->value = header.value;

        LOG(INFO)<< "Type : " << header.type <<", Value : " << header.value << ", Index : "<< count_header;

    }

    return r;
}


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
        std::size_t reply_length;
        unsigned int count_index = 0;
        char *ch;
				
				//Key : Type of header, Value : Information
        std::vector<struct http_header *> header_vec(HEADER_RECV_DB);

        if((reply_length = asio::read(*sock, asio::buffer(buffer_recv, FILE_BUFFER))) > 0) {
            //Get Internet Header details.
            std::string buffer_str(buffer_recv);
            std::vector<std::string> header_detail_vec;

            std::string pattern_1("\n");
            size_t index = 0;
            size_t index_temp  = 0;
            uint8_t count_header = 0;

            while((index=buffer_str.find_first_of(pattern_1)) != std::string::npos) {

                if(index_temp != index) {

                    std::string detail =  buffer_str.substr(index_temp, index);
                    header_detail_vec.push_back(detail);
										header_vec[count_header] = new http_header;
                    bool ret_parse = parse_header_cvd(detail, header_vec, count_header);

                    //LOG(INFO)<< "Header substring : " << detail <<" , Parser : " << ret_parse;
                }

                buffer_str = buffer_str.substr(0, index)+buffer_str.substr(index+pattern_1.size());
                index_temp =  index;
                count_header++;

                if(count_header == HEADER_INTERNAL_PARSE)
                    break;
            }

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

            //TODO: Header check error before next steps.
        }

				struct http_header * header_detail = header_vec[HTTP_CONTAIN_LENGTH];
				boost::algorithm::trim(header_detail->value);
			  uint64_t totalsize = boost::lexical_cast<uint64_t>(header_detail->value);

				LOG(INFO)<<" Header detail, Container size : " <<totalsize; 

        //Get all body of header write to file.
        while ( (reply_length = asio::read(*sock, asio::buffer(buffer_recv, FILE_BUFFER))) > 0) {

            char *end = buffer_recv + FILE_BUFFER;  //Fix size of buffer. Strlen(buffer_recv) not use.
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
