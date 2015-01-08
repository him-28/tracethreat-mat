#ifndef INTERNET_SECURITY_AES_ENCRYPTION_HPP
#define INTERNET_SECURITY_AES_ENCRYPTION_HPP

#include "internet/security/encryption.hpp"
//#include "internet/security/aes_controller.hpp"

namespace internet
{

    namespace security
    {

        //Mookup class
        class aes_encryption : public encryption
        {

            public :

                aes_encryption() :
                 aes_(new aes_controller<message_scan::ResponseScan, aes_cbc>()) {
                    LOG(INFO)<<"Security AES initial support security module.";
                }

                ~aes_encryption() { }


                bool initial_engine() {
                    return aes_->initial_engine();
                }

                internet::security::aes_cbc *
                initial_key(std::string ip, std::string uuid) {
                    return aes_->initial_key(ip, uuid);
                }

                bool find_key(const char *ip, const char *uuid) {
                    return aes_->find_key(ip, uuid);
                }

                bool encryption_msgs(const char *msg, int msg_length) {
                    return aes_->encryption_msgs(msg, msg_length, enc_type);
                }

                bool decryption_msgs(const char *msg, int msg_length) {
                    if(enc_type == NULL)
                        return false;

                    return aes_->decryption_msgs(msg, msg_length, enc_type);
                }//decryption

                internet::security::encryption_controller<internet::security::aes_cbc> *  
                get_encryption() {
                    return aes_;
                }

                internet::security::aes_cbc *
								filter_key(const char * ip, const char * uuid){
										return aes_->filter_key(ip, uuid);
								}

            private:
                //encryption_controller<aes_cbc> * aes_;//encryption_contr;
                //boost::shared_ptr<aes_controller<message_scan::ResponseScan, aes_cbc> >  aes_;
                aes_controller<message_scan::ResponseScan, aes_cbc> * aes_;
                aes_cbc *enc_type;

        };


        REGISTER_ENCRYPTION(aes_encryption);


    }//security

}//internet

#endif /* INTERNET_SECURITY_AES_ENCRYPTION_HPP */
