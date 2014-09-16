#include "internet/scan_client/scan_client.hpp"

#include "utils/uuid_generator.hpp"
#include "utils/base/common.hpp"

#include <vector>

TEST(ScanClient, connection_client)
{

    std::vector<utils::file_scan_request *>  fs_request_vec;

    utils::file_scan_request *fs;
    fs = new utils::file_scan_request;
    fs->binary = std::string("e5949a143be892323217b183e13a8789bc328e");
    fs->file_name = std::string("/home/testlab/zbot-test1.exe");
    fs->file_size = 50000;
    fs->scan_type = message_scan::RequestScan::MD5;
    fs->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs);


    std::string url_addr("127.0.0.1");
    std::string port("4050");
    boost::asio::io_service io_service;
    internet::service::scan_client::start(io_service,url_addr, port, fs_request_vec);
    io_service.run();

    /*
    fs = fs_request_vec[1];
    fs->binary = std::string("e5b49a143be892323217b183e13a8789bc328e");
    fs->file_name = std::string("/home/testlab/zbot-test2.exe");
    fs->file_size = 50000;
    fs->scan_type = message_scan::RequestScan::MD5;
    fs->file_type = message_scan::RequestScan::ELF;

    fs = fs_request_vec[2];
    fs->binary = std::string("e5c49a143be892323217b183e13a8789bc328e");
    fs->file_name = std::string("/home/testlab/zbot-test4.exe");
    fs->file_size = 50000;
    fs->scan_type = message_scan::RequestScan::MD5;
    fs->file_type = message_scan::RequestScan::PROCESS;
    */

}


