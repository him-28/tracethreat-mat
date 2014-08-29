#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "utils/file_handler.cpp"

#include "internet/client_fresh/updated_sign.hpp"


#define MEM_FN(X)					boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)			boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x, y, z)	boost::bind(&self_type::x, shared_from_this(), y, z)

#define HEADER_RECV_DB  10
#define HEADER_INTERNAL_PARSE 7
#define HTTP_CONTAIN_LENGTH 6

BOOST_FUSION_ADAPT_STRUCT(
        internet::http_header,
        (std::string, type)
        (std::string, value)
)

namespace internet
{

    bool updated_sign::write_sign(std::vector<char> *buffer_vec)
    {
        LOG(INFO)<<" Write Signature to file path : "<<file_path;

        utils::file_handler<utils::common_filetype>  fh;
        fh.set_filepath(file_path);
        fh.write_file(*buffer_vec);
    }


    bool updated_sign::parse_header_cvd(std::string const&  header_str,
            std::vector<http_header *>& header_vector,
            uint8_t count_header)
    {

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


            //Trim string.
            boost::algorithm::trim(header.value);

            http->value = header.value;

            LOG(INFO)<< "Type : " << header.type <<", Value : " << header.value;
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

		void updated_sign::on_read_body(const error_code& error, size_t  bytes)
		{
					LOG(INFO)<<" On_read_body ,Size : " << bytes;
					LOG(INFO)<<" Buffer : "<< read_buffer_;
		}

		size_t updated_sign::read_body_completion(const error_code & error, size_t bytes)
		{
				LOG(INFO) <<" Read_body_completion, Size :" << bytes;
	
		}

    void updated_sign::on_read_header(const error_code& error, size_t bytes, socket * sock)
    {
        try {

            LOG(INFO)<<" On_read_header read byte[ "<< bytes <<" ] ";

            //Get Internet Header details.
            std::string buffer_str(read_buffer_);
            std::vector<std::string> header_detail_vec;

					char * end = read_buffer_ + strlen(read_buffer_);
					buffer_vec.insert(buffer_vec.begin(), read_buffer_, end);
					write_sign(&buffer_vec);


            unsigned int count_index = 0;
            char *ch;
            //Key : Type of header, Value : Information
            std::vector<struct http_header *> header_vec(HEADER_RECV_DB);

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
                }

                buffer_str = buffer_str.substr(0, index)+buffer_str.substr(index+pattern_1.size());
                index_temp =  index;
                count_header++;

                if(count_header == HEADER_INTERNAL_PARSE)
                    break;
            }

            ch = read_buffer_;

            while(1) {
                if( count_index > 2 && *ch == '\n' &&
                        *(ch - 1) == '\r' &&
                        *(ch - 2) == '\n' &&
                        *(ch - 3) == '\r') {
                    LOG(INFO) <<" End index of header : " << count_index;
                    count_index++;
                    break;
                }

                count_index++;
                ch++;
            }//End while get buffer

						//Get body information write to  buffer 
					   asio::async_read(*sock,
                    asio::buffer(read_buffer_),	
                    boost::bind(&updated_sign::read_body_completion,
                            this,
                            asio::placeholders::error,
                            asio::placeholders::bytes_transferred
                               ),
                    boost::bind(&updated_sign::on_read_body,
                            this,
                            asio::placeholders::error,
                            asio::placeholders::bytes_transferred
                               ));



        } catch(boost::system::system_error e) {

            LOG(INFO)<<" On_read_hander exception : "<< e.code();
        }

    }

    void updated_sign::read_header(const error_code& error, socket *sock)
    {
        try {
            //first read header for get data.
            asio::async_read(*sock,
                    asio::buffer(read_buffer_),
                    asio::transfer_exactly(FILE_BUFFER),
                    boost::bind(&updated_sign::on_read_header,
                            this,
                            asio::placeholders::error,
                            asio::placeholders::bytes_transferred,
														sock
                               ));

        } catch(boost::system::system_error e) {
            LOG(INFO)<<" Read_header exception : "<< e.code();
        }
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
            sock->async_write_some(asio::buffer(cmd, strlen(cmd)),
                    boost::bind(&updated_sign::read_header, this, asio::placeholders::error, sock));

        } catch(boost::system::system_error er) {
            LOG(INFO)<<"Error in send command : " << er.code();
        }

    }

}// end namespace


