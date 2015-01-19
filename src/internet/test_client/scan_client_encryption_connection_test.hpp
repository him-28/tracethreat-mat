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
    utils::file_scan_request *fs;

    fs = new utils::file_scan_request;
    fs->binary = std::string("f33867592e3ecf226eb7b32fcdf8a53123433333223444a32e232323ae54a");
    fs->file_name = std::string("/home/testlab/zbot-test2.exe");
    fs->file_size = 50000;
    fs->scan_type = message_scan::RequestScan::MD5;
    fs->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs);

    fs = new utils::file_scan_request;
    fs->binary = std::string("e5c49a143be892323217b183e13a8789bc328e");
    fs->file_name = std::string("/home/testlab/zbot-test4.exe");
    fs->file_size = 50000;
    fs->scan_type = message_scan::RequestScan::MD5;
    fs->file_type = message_scan::RequestScan::PE;
    fs_request_vec.push_back(fs);

    std::string url_addr("127.0.0.1");
    std::string port("4050");
    boost::asio::io_service io_service;
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
