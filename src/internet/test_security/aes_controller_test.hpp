#include "internet/security/aes_controller.hpp"

#include "internet/msg/packedmessage_scan.hpp"

#include <boost/thread/thread.hpp>
#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include  <memory>
#include <string.h>
#include <string>
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





void thread_initial_key(internet::security::encryption_controller<internet::security::aes_cbc>
  & enc_controller){
  	std::string ip_prefix[5] = "192.168.1.";
    std::string uuid_prefix[5] = "172:2343:22336:";
 	  for(int count_key = 0; count_key < 5; count_key++){

	 			std::string ip_suffing = std::to_string(count_key);
	      ip_prefix[count_key].append(ip_suffing);
       
        std::string uuid_suffing  = std::to_string(count_key);
        uuid_prefix[count_key].append(uuid_suffing); 	
	
				if(enc_controller.filter_key(ip_prefix[count_key].c_str(), uuid_prefix[count_key].c_str())){
           internet::security::aes_cbc *aes_cbc_ptr = 
               enc_controller.initial_key(ip_prefix[count_key], uuid_prefix[count_key]);

			     printf("--- key hex : \t");
           hex_print((unsigned char *)&aes_cbc_ptr->key, KEY_SIZE);

           printf("--- iv hex : \t");
           hex_print((unsigned char *)&aes_cbc_ptr->iv, AES_BLOCK_SIZE);
				}else{
						LOG(INFO)<<"IP is stored in module. IP : "<<ip_prefix[count_key];
				} 

		}//for 
}

TEST(aescontroller, aes_encryption_thread){


    //internet::security::encryption_controller<internet::security::aes_cbc>  * enc_controller = new 
    //internet::security::aes_controller<message_scan::ResponseScan, internet::security::aes_cbc>();

    internet::security::aes_controller<message_scan::ResponseScan, internet::security::aes_cbc> 
			aes_enc_initial;

    aes_enc_initial.initial_engine();

    internet::security::encryption_controller<internet::security::aes_cbc>  * enc_controller = 
		&aes_enc_initial;
  
    boost::thread t1(boost::bind(&thread_initial_key, boost::ref(*enc_controller)));
    boost::thread t2(boost::bind(&thread_initial_key, boost::ref(*enc_controller)));
    boost::thread t3(boost::bind(&thread_initial_key, boost::ref(*enc_controller)));

		t1.join();
		t2.join();
		t3.join();

}

/*
TEST(aescontroller, encryptionstring)
{
		//http://stackoverflow.com/queto_stringns/5727646/what-is-the-length-parameter-of-aes-evp-decrypt 
    //set ip, uuid

    std::string ip("127.0.0.1");
    std::string uuid("172:2343:22336:233");
    internet::security::encryption_controller<internet::security::aes_cbc>  * enc_controller = new 
    	internet::security::aes_controller<message_scan::ResponseScan, internet::security::aes_cbc>();

 internet::security::aes_controller<message_scan::ResponseScan, internet::security::aes_cbc> aes_enc_initial;



    aes_enc_initial.initial_engine();

    internet::security::aes_cbc *aes_cbc_ptr = enc_controller->initial_key(ip, uuid);
	
		std::string msg_test("172:2343:22336:233");
    printf("--- key hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->key, KEY_SIZE);

    printf("--- iv hex : \t");
    hex_print((unsigned char *)&aes_cbc_ptr->iv, AES_BLOCK_SIZE);

	  printf("--- plain text hex : \t");
    hex_print((unsigned char *)msg_test.c_str(), msg_test.size());


    enc_controller->encryption_msgs(msg_test.c_str(), msg_test.size(), aes_cbc_ptr);

	  printf("--- encryption success text hex : \t");
    hex_print((unsigned char *)aes_cbc_ptr->enc_msg, aes_cbc_ptr->enc_length);
	
		std::string msg_test_decrypt(aes_cbc_ptr->enc_msg, aes_cbc_ptr->enc_msg+ aes_cbc_ptr->enc_length);
		
		LOG(INFO)<<"Message_test_Encryption : " << msg_test_decrypt;

		enc_controller->decryption_msgs(msg_test_decrypt.c_str(), msg_test_decrypt.size(), aes_cbc_ptr);

    printf("message after decrypt hex : \t");
    hex_print((unsigned char *)aes_cbc_ptr->dec_msg, aes_cbc_ptr->input_length);

		LOG(INFO)<<"Message : " << aes_cbc_ptr->dec_msg;

    //bool res = enc_controller->find_key(ip, uuid);
    //LOG(INFO)<<" found key true : " << res;

		bool res_filter = enc_controller->filter_key(ip.c_str(), uuid.c_str());
		LOG(INFO)<<" Filter key is true : " << res_filter;

		std::string ip_false("192.168.1.1");
		std::string uuid_false("0:0:0:0");
 		bool res_filter_false = enc_controller->filter_key(ip_false.c_str(), uuid_false.c_str());
		LOG(INFO)<<" Filter key is true : " << res_filter_false;


}
*/







