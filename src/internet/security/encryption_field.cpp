#include <typeinfo>

#include "internet/security/encryption_field.hpp"

namespace internet
{

    namespace security
    {

        template<typename MessageType, typename EncryptType>
        bool scan_field<MessageType, EncryptType>::encryption(MsgsPointer msg,
                encrypt_type *enc_controller)
        {
            //LOG(INFO)<<"Message encrypted : IP : "<< msg->ip()<<", UUID : " << msg->uuid();
            //encryption steps.
            internet::security::aes_cbc *aes =
                    enc_controller->process_crypto(msg->conn_ip(),
                            msg->conn_uuid(),
                            utils::crypto_mode::find_key_crypto_mode);

            //LOG(INFO)<<"Result find IP : " << aes->ip<<", UUID : "<< aes->uuid;

            if(aes == NULL) {
                //internet::security::aes_cbc *
                aes = enc_controller->process_crypto(msg->ip(), msg->uuid(),
                        utils::crypto_mode::insert_key_crypto_mode);

                LOG(INFO)<<"Not found initial key in system. Initial key...";
                //LOG(INFO)<<" Initial key : "<< aes->key;
                //LOG(INFO)<<" IV          : "<< aes->iv;
            }

            //Set field
            //UUID
            if(!msg->uuid().empty()) {
                const char *uuid[msg->uuid().size()];
                enc_controller->encryption_msgs(msg->uuid().c_str(), msg->uuid().size(), aes);
                msg->set_uuid((const char *)aes->enc_msg);
                aes->enc_msg = 0;
            }

            //IP
            if(!msg->ip().empty()) {
                const char *ip[msg->uuid().size()];
                enc_controller->encryption_msgs(msg->ip().c_str(), msg->ip().size(), aes);
                msg->set_ip((const char *)aes->enc_msg);
                aes->enc_msg = 0;
            }

            //TIMESTAMP
            if(!msg->timestamp().empty()) {
                const char *timestamp[msg->timestamp().size()];
                enc_controller->encryption_msgs(msg->timestamp().c_str(), msg->timestamp().size(), aes);
                msg->set_timestamp((const char *)aes->enc_msg);
                aes->enc_msg = 0;
            }


            LOG(INFO)<<"Binary size : " << msg->set_binary_value_size();

            //binary in loop of repeated
            for(int count_msg = 0;
                    count_msg <  msg->set_binary_value_size();
                    count_msg++) {
                //get value.
                if(typeid(message_scan::ResponseScan::SetBinaryValue) ==
                        typeid(*msg->mutable_set_binary_value(count_msg))) {

                    message_scan::ResponseScan::SetBinaryValue *msg_resp_binary =
                            (message_scan::ResponseScan::SetBinaryValue *)
                            msg->mutable_set_binary_value(count_msg);

                    if(msg_resp_binary->binary().empty())
                        continue;

                    LOG(INFO)<<"BINARY : "<< msg_resp_binary->binary();
                    //Encryption binary
                    const char *binary[msg_resp_binary->binary().size()];
                    enc_controller->encryption_msgs(msg_resp_binary->binary().c_str(),
                            msg_resp_binary->binary().size(), aes);
                    msg_resp_binary->set_binary((const char *)aes->enc_msg);
                    aes->enc_msg = 0;

                } else if(typeid(message_scan::RequestScan::SetBinaryValue) ==
                        typeid(*msg->mutable_set_binary_value(count_msg))) {

                    message_scan::RequestScan::SetBinaryValue *msg_req_binary =
                            (message_scan::RequestScan::SetBinaryValue *)
                            msg->mutable_set_binary_value(count_msg);

                    if(msg_req_binary->binary().empty())
                        continue;

                    LOG(INFO)<<"BINARY : "<< msg_req_binary->binary();

                    //Encryption binary
                    const char *binary[msg_req_binary->binary().size()];
                    enc_controller->encryption_msgs(msg_req_binary->binary().c_str(),
                            msg_req_binary->binary().size(), aes);
                    msg_req_binary->set_binary((const char *)aes->enc_msg);
                    aes->enc_msg = 0;

                } else {
                    LOG(INFO)<<"Not type support template.";
                    return false;
                }

            }//for


            return true;
        }//encryption

        template<typename MessageType, typename EncryptType>
        bool scan_field<MessageType, EncryptType>::decryption(MsgsPointer msg,
                encrypt_type *enc_controller)
        {
            //[-] read encryption size.
            //[-] create dec_msg from malloc(aes->enc_length);
            //[-] message decrypted.
            internet::security::aes_cbc *aes =
                    enc_controller->process_crypto(msg->conn_ip(),
                            msg->conn_uuid(),
                            utils::crypto_mode::find_key_crypto_mode );

            if(aes != NULL) {

                //LOG(INFO)<<"AES key retrived : "<< aes->key;
                //LOG(INFO)<<"AES IV  retrived : "<< aes->iv;
                //UUID
                //LOG(INFO)<<"UUID : "<<msg->uuid();
                aes->enc_length =  msg->uuid().size();

                aes->enc_msg  = (unsigned char *)malloc(msg->uuid().size() + MAX_PADDING_LEN);
                std::copy(msg->uuid().begin(), msg->uuid().end(), aes->enc_msg);
                aes->enc_msg[msg->uuid().size()] = '\0';

                enc_controller->decryption_msgs(msg->uuid().c_str(), msg->uuid().size() , aes);
                msg->set_uuid((const char *)aes->dec_msg);

                //IP
                //Check lenth from encryption length.
                //LOG(INFO)<<"IP : "<<msg->ip();
                aes->enc_length =  msg->ip().size();

                aes->enc_msg  = (unsigned char *)malloc(msg->ip().size() + MAX_PADDING_LEN);
                std::copy(msg->ip().begin(), msg->ip().end(), aes->enc_msg);
                aes->enc_msg[msg->ip().size()] = '\0';

                enc_controller->decryption_msgs(msg->ip().c_str(), msg->ip().size() , aes);
                msg->set_ip((const char *)aes->dec_msg);

                //TIMESTAMP
                //Check lenth from encryption length.
                //LOG(INFO)<<"TIMESTAMP : "<< msg->timestamp();
                aes->enc_length =  msg->timestamp().size();

                aes->enc_msg  = (unsigned char *)malloc(msg->timestamp().size() + MAX_PADDING_LEN);
                std::copy(msg->timestamp().begin(), msg->timestamp().end(), aes->enc_msg);
                aes->enc_msg[msg->timestamp().size()] = '\0';

                enc_controller->decryption_msgs(msg->timestamp().c_str(), msg->timestamp().size() , aes);
                msg->set_timestamp((const char *)aes->dec_msg);

                //binary in loop of repeated
                for(int count_msg = 0;
                        count_msg <  msg->set_binary_value_size();
                        count_msg++) {
                    //get value.
                    if(typeid(message_scan::ResponseScan::SetBinaryValue) ==
                            typeid(*msg->mutable_set_binary_value(count_msg))) {

                        message_scan::ResponseScan::SetBinaryValue *msg_resp_binary =
                                (message_scan::ResponseScan::SetBinaryValue *)
                                msg->mutable_set_binary_value(count_msg);

                        if(msg_resp_binary->binary().empty())
                            continue;

                        //LOG(INFO)<<"BINARY : "<< msg_resp_binary->binary();
                        aes->enc_length = msg_resp_binary->binary().size();

                        aes->enc_msg  =
                                (unsigned char *)malloc(msg_resp_binary->binary().size() + MAX_PADDING_LEN);
                        std::copy(msg_resp_binary->binary().begin(),
                                msg_resp_binary->binary().end(),
                                aes->enc_msg);
                        aes->enc_msg[msg_resp_binary->binary().size()] = '\0';

                        enc_controller->decryption_msgs(msg_resp_binary->binary().c_str(),
                                msg_resp_binary->binary().size() , aes);
                        msg_resp_binary->set_binary((const char *)aes->dec_msg);

                        //LOG(INFO)<<"DECRYPTION RESPONSE BINARY : " << msg_resp_binary->binary();

                    } else if(typeid(message_scan::RequestScan::SetBinaryValue) ==
                            typeid(*msg->mutable_set_binary_value(count_msg))) {

                        message_scan::RequestScan::SetBinaryValue *msg_req_binary =
                                (message_scan::RequestScan::SetBinaryValue *)
                                msg->mutable_set_binary_value(count_msg);

                        if(msg_req_binary->binary().empty())
                            continue;

                        //LOG(INFO)<<"BINARY : "<< msg_req_binary->binary();

                        //Binary decryption

                        aes->enc_msg  =
                                (unsigned char *)malloc(msg_req_binary->binary().size() + MAX_PADDING_LEN);
                        std::copy(msg_req_binary->binary().begin(),
                                msg_req_binary->binary().end(),
                                aes->enc_msg);
                        aes->enc_msg[msg_req_binary->binary().size()] = '\0';

                        enc_controller->decryption_msgs(msg_req_binary->binary().c_str(),
                                msg_req_binary->binary().size() , aes);
                        msg_req_binary->set_binary((const char *)aes->dec_msg);

                        //LOG(INFO)<<"DECRYPTION REQUEST BINARY : " << msg_req_binary->binary();

                    } else {
                        LOG(INFO)<<"Not type support template.";
                        return false;
                    }

                }//for



                return true;
            }//aes not null

            return false;
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


