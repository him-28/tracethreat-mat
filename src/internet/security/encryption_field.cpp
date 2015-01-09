#include "internet/security/encryption_field.hpp"

namespace internet
{

    namespace security
    {

        template<typename MessageType, typename EncryptType>
        bool scan_field<MessageType, EncryptType>::encryption(MsgsPointer msg,
                encrypt_type *enc_controller)
        {
            //encryption steps.
            internet::security::aes_cbc *aes = 
            enc_controller->filter_key(msg->ip().c_str(), msg->uuid().c_str());

            if(aes == NULL)
                return false;

            //Set field
            //UUID
            if(!msg->uuid().empty()) {
                const char *uuid[msg->uuid().size()];
                enc_controller->encryption_msgs(msg->uuid().c_str(), msg->uuid().size(), aes);
                msg->set_uuid((const char*)aes->enc_msg);
                aes->enc_msg = 0;
            }
						/* /IP
            if(!msg->ip().empty()) {
                const char *ip[msg->uuid().size()];
                enc_controller->encrypt_msgs(msg->ip().c_str(), msg->ip().size(), aes);
                msg->set_ip(aes->enc_msg);
                aes->enc_msg = 0;
            }
            */
             
						/* /BINARY
            if(!msg->binary().empty()) {
                const char *binary[msg->binary().size()];
                enc_controller->encrypt_msgs(msg->binary().c_str(), msg->binary().size(), aes);
                msg->set_binary(aes->enc_msg);
                aes->enc_msg = 0;
            }
						*/
						//TIMESTAMP
            if(!msg->timestamp().empty()) {
                const char *timestamp[msg->timestamp().size()];
                enc_controller->encryption_msgs(msg->timestamp().c_str(), msg->timestamp().size(), aes);
                msg->set_timestamp((const char*)aes->enc_msg);
                aes->enc_msg = 0;
            }

            return true;
        }//encryption

        template<typename MessageType, typename EncryptType>
        bool scan_field<MessageType, EncryptType>::decryption(MsgsPointer msg,
                encrypt_type *enc_controller)
        {

        }//decryption

        template<typename MessageType, typename EncryptType>
        internet::security::aes_cbc *scan_field<MessageType, EncryptType>::
        filter_key(const char *uuid, const char *ip)
        {


        }//filter key

				//template define 
				template class scan_field<message_scan::ResponseScan, internet::security::aes_cbc>;
	
				template class scan_field<message_scan::RequestScan, internet::security::aes_cbc>;
				


    }//security

}//internet


