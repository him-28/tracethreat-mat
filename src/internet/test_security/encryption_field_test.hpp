#include "internet/security/encryption.hpp"

#include "internet/security/encryption_field.hpp"

#include "internet/security/load_security.hpp"

class EncryptionLoadEngine : public ::testing::Test{

		protected:

		typedef internet::security::encryption_controller<internet::security::aes_cbc> encryption_type;

		virtual void SetUp(){
				                //Crypto and Network Security
                LOG(INFO)<<"Server : Load security module.";
                internet::security::get_encryption().reset(
                        internet::security::create_encryption());

                if(internet::security::get_encryption().get() == NULL) {
                    LOG(INFO)<<"System cannot initial encryption engine";
                    //return false;
                }

								enc_controller_ = internet::security::get_encryption()->get_encryption();
								if(enc_controller_ == NULL){
									LOG(INFO)<<"Encryption controller cannot intial";
								}
	

		}

	
		encryption_type * enc_controller_; 
	

};



TEST_F(EncryptionLoadEngine, EncryptionField){

		internet::security::secure_field<message_scan::ResponseScan, internet::security::aes_cbc> * sec_field = 
        new internet::security::scan_field<message_scan::ResponseScan, internet::security::aes_cbc>();

		boost::shared_ptr<message_scan::ResponseScan> msg_response(new message_scan::ResponseScan);

		msg_response->set_uuid(std::string());
    //msg_response->set_ip(std::string());
		//msg_response->set_binary(std::string());
    msg_response->set_timestamp(std::string());

		sec_field->encryption(msg_response, enc_controller_);
     
		LOG(INFO)<<"UUID : "<<msg_response->uuid();
		//LOG(INFO)<<"IP   : "<<msg_response->ip();
    //LOG(INFO)<<"BINARY : "<<msg_response->binary();
    LOG(INFO)<<"TIMESTAMP : " <<msg_response->timestamp();

}
