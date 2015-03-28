#include <boost/scoped_ptr.hpp>

#include "internet/security/encryption.hpp"

#include "internet/security/encryption_field.hpp"

#include "internet/security/load_security.hpp"

#include "internet/scan_client/scan_client.hpp"

#include "utils/uuid_generator.hpp"
#include "utils/base/common.hpp"
#include <vector>

class ScanClientEncryptionLoadEngine : public ::testing::Test
{

    protected:

        typedef internet::security::encryption_controller<internet::security::aes_cbc> encryption_type;

        virtual void SetUp() {

            //Crypto and Network Security
            LOG(INFO)<<"Client : Load security module.";
            internet::security::get_encryption().reset(
                    internet::security::create_encryption());

            if(internet::security::get_encryption().get() == NULL) {
                LOG(INFO)<<"Client : System cannot initial encryption engine";
                //return false;
            }

            enc_controller_ = internet::security::get_encryption()->get_encryption();
            internet::security::get_encryption()->initial_engine();

            if(enc_controller_ == NULL) {
                LOG(INFO)<<"Client : Encryption controller cannot intial";
            }


        }


        encryption_type *enc_controller_;


};

TEST_F(ScanClientEncryptionLoadEngine, connection_register_crypto)
{

    std::vector<utils::file_scan_request *> fs_request_vec;
    utils::file_scan_request *fs1;

    utils::file_scan_request *fs2;

    utils::file_scan_request *fs3;

    fs1 = new utils::file_scan_request;
    fs1->binary = std::string("f33867592e3ecf226eb7b32fcdf8a53123433333223444a32e232323ae54a");
    fs1->file_name = std::string("/home/testlab/zbot-test2.exe");
    fs1->uuid      = std::string("0:0:0:1");
    fs1->machine_name = std::string("test-machine-01");
    fs1->ip        = std::string("127.0.0.1");
    fs1->file_size = 50000;
    fs1->scan_type = message_scan::RequestScan::MD5;
    fs1->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs1);

    fs2 = new utils::file_scan_request;
    fs2->binary = std::string("e1aef4e5c49a143be892323217b183e13a8789bc328e");
    fs2->uuid      = std::string("0:0:0:2");
    fs2->machine_name = std::string("test-machine-02");
    fs2->ip        = std::string("127.0.0.2");
    fs2->file_name = std::string("/home/testlab/zbot-test4.exe");
    fs2->file_size = 50000;
    fs2->scan_type = message_scan::RequestScan::MD5;
    fs2->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs2);

    fs3 = new utils::file_scan_request;
    fs3->binary = std::string("f33867592e3ecf226eb7b32fcdf8a53123433333223444a32e232323ae54a");
    fs3->uuid      = std::string("0:0:0:3");
    fs3->machine_name = std::string("test-machine-03");
    fs3->ip        = std::string("127.0.0.3");
    fs3->file_name = std::string("/home/testlab/zbot-test3.exe");
    fs3->file_size = 50000;
    fs3->scan_type = message_scan::RequestScan::MD5;
    fs3->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs3);



    std::string port("4050");
    boost::asio::io_service io_service;

    std::string url_addr;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query("localhost", "");

    // prepare response iterator
    boost::asio::ip::tcp::resolver::iterator destination = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator iter_endpoint;
    boost::asio::ip::tcp::endpoint endpoint;


    while (destination != iter_endpoint) {
        endpoint = *destination++;
        //addresses.push_back(endpoint.address().to_string());
        url_addr = endpoint.address().to_string();
    }


    boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
    context.load_verify_file("ca.pem");
    internet::service::scan_client::start(io_service,
            context,
            url_addr,
            port,
            fs_request_vec,
            enc_controller_);
    io_service.run();


}
