#include "internet/security/aes_controller.hpp"

#include "internet/msg/packedmessage_scan.hpp"

#include  <memory>
#include <string.h>
// a simple hex-print routine. could be modified to print 16 bytes-per-line
static void hex_print(const void *pv, size_t len)
{
    const unsigned char *p = (const unsigned char *)pv;
    //const uint8_t *p = (const uint8_t *)pv;

    if (NULL == pv)
        printf("NULL");
    else {
        size_t i = 0;

        for (; i<len; ++i)
            printf("%02X ", *p++);
    }

    printf("\n");
}
/*
TEST(AESController, InitialKeyIv)
{

    std::string ip("127.0.0.1");
    std::string uuid("172:2343:22336:233");
    internet::security::aes_controller<message_scan::RequestScan> aes_enc_initial;
    aes_enc_initial.initial_engine();
    internet::security::aes_cbc *aes_cbc_ptr = aes_enc_initial.initial_key(ip, uuid);

    printf("--- KEY HEX : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->key, KEY_SIZE);

    printf("--- IV HEX : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->iv, AES_BLOCK_SIZE);

    bool res = aes_enc_initial.find_key(ip, uuid);
    LOG(INFO)<<" Found key true : " << res;

    std::string ip_false("127.0.0.2");
    std::string uuid_false("172:2343:22336:233");
    bool res_false = aes_enc_initial.find_key(ip_false, uuid_false);
    LOG(INFO)<<" Found key true : " << res_false;

    std::string ip_false2("127.0.0.1");
    std::string uuid_false2("172:2343:22336:243");
    bool res_false2 = aes_enc_initial.find_key(ip_false2, uuid_false2);
    LOG(INFO)<<" Found key true : " << res_false2;

    std::string ip_false3("122.0.0.1");
    std::string uuid_false3("173:2343:22336:243");
    bool res_false3 = aes_enc_initial.find_key(ip_false3, uuid_false3);
    LOG(INFO)<<" Found key true : " << res_false3;


}
*/

/*
TEST(AESController, EncryptionKey)
{
 
    //set IP, UUID
    std::string ip("127.0.0.1");
    std::string uuid("172:2343:22336:233");
    internet::security::aes_controller<message_scan::ResponseScan> aes_enc_initial;

    aes_enc_initial.initial_engine();

    internet::security::aes_cbc *aes_cbc_ptr = aes_enc_initial.initial_key(ip, uuid);

		boost::shared_ptr<message_scan::ResponseScan> scan_response(new message_scan::ResponseScan);
    scan_response->set_uuid(uuid);
    scan_response->set_type(message_scan::ResponseScan::SCAN_SUCCESS);
    scan_response->set_timestamp(std::string("0:0:1:0"));
		
		std::vector<uint8_t> msg_vec;

		internet::packedmessage_scan<message_scan::ResponseScan> resp_msg(scan_response);


	  printf("--- Plain text before pack HEX : \t");
    hex_print((uint8_t *)&msg_vec.front(), msg_vec.size());


	  resp_msg.pack(msg_vec);

    printf("--- KEY HEX : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->key, KEY_SIZE);

    printf("--- IV HEX : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->iv, AES_BLOCK_SIZE);

	   printf("--- Plain text HEX : \t");
    hex_print((uint8_t *)&msg_vec.front(), msg_vec.size());

	   printf("--- Plain text HEX : \t");
    hex_print((uint8_t *)msg_vec.data(), msg_vec.size());


    aes_enc_initial.encryption_msgs(msg_vec, aes_cbc_ptr);

//    printf("--- ENC MSG HEX : \t");
//    hex_print((unsigned char *)&aes_cbc_ptr->enc_msg, aes_cbc_ptr->enc_length);

    bool res = aes_enc_initial.find_key(ip, uuid);
    LOG(INFO)<<" Found key true : " << res;

		std::vector<uint8_t>  msg_vec_result;

		aes_enc_initial.decryption_msgs(msg_vec_result, aes_cbc_ptr);

		boost::shared_ptr<message_scan::ResponseScan> scan_response_ret(new message_scan::ResponseScan);
		internet::packedmessage_scan<message_scan::ResponseScan> resp_msg_ret(scan_response_ret);
		resp_msg_ret.unpack(msg_vec_result);

		if(scan_response_ret = resp_msg_ret.get_msg())
			LOG(INFO)<<" UUID : " << scan_response_ret->uuid();

}
*/
/*
test(aescontroller, encryptionmessage)
{
 
    //set ip, uuid
    std::string ip("127.0.0.1");
    std::string uuid("172:2343:22336:233");
    internet::security::aes_controller<message_scan::responsescan> aes_enc_initial;

    aes_enc_initial.initial_engine();

    internet::security::aes_cbc *aes_cbc_ptr = aes_enc_initial.initial_key(ip, uuid);

		boost::shared_ptr<message_scan::responsescan> scan_response(new message_scan::responsescan);
    scan_response->set_uuid(uuid);
    scan_response->set_type(message_scan::responsescan::scan_success);
    scan_response->set_timestamp(std::string("0:0:1:0"));
		
    printf("--- key hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->key, key_size);

    printf("--- iv hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->iv, aes_block_size);

	  printf("--- plain text hex : \t");
    hex_print((uint8_t *)&scan_response,  scan_response->bytesize());


    aes_enc_initial.encryption_msgs(scan_response, aes_cbc_ptr);


    bool res = aes_enc_initial.find_key(ip, uuid);
    log(info)<<" found key true : " << res;

		std::vector<uint8_t>  msg_vec_result;

		boost::shared_ptr<message_scan::responsescan> scan_response_ret(new message_scan::responsescan);

		aes_enc_initial.decryption_msgs(scan_response_ret, aes_cbc_ptr);
		log(info)<<"uuid : "<< scan_response_ret->uuid();

}
*/

/*
test(aescontroller, encryptionstring)
{
 
    //set ip, uuid

    std::string ip("127.0.0.1");
    std::string uuid("172:2343:22336:233");
    internet::security::aes_controller<message_scan::responsescan> aes_enc_initial;

    aes_enc_initial.initial_engine();

    internet::security::aes_cbc *aes_cbc_ptr = aes_enc_initial.initial_key(ip, uuid);

//		boost::shared_ptr<message_scan::responsescan> scan_response(new message_scan::responsescan);
//    scan_response->set_uuid(uuid);
//    scan_response->set_type(message_scan::responsescan::scan_success);
//    scan_response->set_timestamp(std::string("0:0:1:0"));
	
		std::string msg_test("172:2343:22336:233");
    printf("--- key hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->key, key_size);

    printf("--- iv hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->iv, aes_block_size);

	  printf("--- plain text hex : \t");
    hex_print((unsigned char *)msg_test.c_str(), msg_test.size());


    aes_enc_initial.encryption_msgs(msg_test.c_str(), msg_test.size(), aes_cbc_ptr);

	  printf("--- encryption success text hex : \t");
    hex_print((unsigned char *)aes_cbc_ptr->enc_msg, aes_cbc_ptr->enc_length);


    bool res = aes_enc_initial.find_key(ip, uuid);
    log(info)<<" found key true : " << res;
		
		std::string msg_test_decrypt(msg_test);
		aes_enc_initial.decryption_msgs(msg_test_decrypt.c_str(), msg_test_decrypt.size(), aes_cbc_ptr);

    printf("message after decrypt hex : \t");
    hex_print((unsigned char *)aes_cbc_ptr->dec_msg, aes_cbc_ptr->input_length);

		log(info)<<" message : " << aes_cbc_ptr->dec_msg;

}
*/




TEST(aescontroller, encryptionstring)
{
 
    //set ip, uuid

    std::string ip("127.0.0.1");
    std::string uuid("172:2343:22336:233");
    internet::security::aes_controller<message_scan::ResponseScan> aes_enc_initial;

    aes_enc_initial.initial_engine();

    internet::security::aes_cbc *aes_cbc_ptr = aes_enc_initial.initial_key(ip, uuid);
	
		std::string msg_test("172:2343:22336:233");
    printf("--- key hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->key, KEY_SIZE);

    printf("--- iv hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->iv, AES_BLOCK_SIZE);

	  printf("--- plain text hex : \t");
    hex_print((unsigned char *)msg_test.c_str(), msg_test.size());


    aes_enc_initial.encryption_msgs(msg_test,aes_cbc_ptr);

	  printf("--- encryption success text hex : \t");
    hex_print((unsigned char *)aes_cbc_ptr->enc_msg, aes_cbc_ptr->enc_length);
	
		std::string msg_test_decrypt(aes_cbc_ptr->enc_msg, aes_cbc_ptr->enc_msg+ aes_cbc_ptr->enc_length);
		
		LOG(INFO)<<"Message_test_Encryption : " << msg_test_decrypt;

		aes_enc_initial.decryption_msgs(msg_test_decrypt, aes_cbc_ptr);

    printf("message after decrypt hex : \t");
    hex_print((unsigned char *)aes_cbc_ptr->dec_msg, aes_cbc_ptr->input_length);

		LOG(INFO)<<"Message : " << aes_cbc_ptr->dec_msg;

    bool res = aes_enc_initial.find_key(ip, uuid);
    LOG(INFO)<<" found key true : " << res;
	
}



