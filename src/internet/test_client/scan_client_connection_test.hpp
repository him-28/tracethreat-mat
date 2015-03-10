#include "internet/scan_client/scan_client.hpp"
#include "utils/uuid_generator.hpp"
#include "utils/base/common.hpp"
#include <vector>
//#include <boost/asio/ssl.hpp>

TEST(ScanClient, connection_client)
{
    std::vector<utils::file_scan_request *> fs_request_vec;
    utils::file_scan_request *fs1;

    utils::file_scan_request *fs2;

    utils::file_scan_request *fs3;


    fs1 = new utils::file_scan_request;
    fs1->binary = std::string("f33867592e3ecf226eb7b32fcdf8a531");
    fs1->file_name = std::string("/home/testlab/zbot-test2.exe");
    fs1->file_size = 50000;
    fs1->scan_type = message_scan::RequestScan::MD5;
    fs1->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs1);

    fs2 = new utils::file_scan_request;
    fs2->binary = std::string("e5c49a143be892323217b183e13a8789bc328e");
    fs2->file_name = std::string("/home/testlab/zbot-test4.exe");
    fs2->file_size = 50000;
    fs2->scan_type = message_scan::RequestScan::MD5;
    fs2->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs2);

    fs3 = new utils::file_scan_request;
    fs3->binary = std::string("f33867592e3ecf226eb7b32fcdf8a532333ab1");
    fs3->file_name = std::string("/home/testlab/zbot-test3.exe");
    fs3->file_size = 50000;
    fs3->scan_type = message_scan::RequestScan::MD5;
    fs3->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs3);


    std::string url_addr("127.0.0.1");
    std::string port("4050");
    boost::asio::io_service io_service;
		boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
		context.load_verify_file("ca.pem");
    internet::service::scan_client::start(io_service, context, url_addr, port, fs_request_vec);
    io_service.run();

}
