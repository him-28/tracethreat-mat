#include "internet/security/aes_controller.hpp"

#include "internet/msg/packedmessage_scan.hpp"

#include <boost/thread/thread.hpp>
#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include  <memory>
#include <string.h>
#include <string>

#include <random>

#define MAX_COUNT 10

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

typedef std::mt19937 RAND;

RAND rng;

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


std::uniform_int_distribution<uint32_t> uint_dist500(0,2);
std::string ip_prefix[MAX_COUNT];
std::string uuid_prefix[MAX_COUNT];
std::string ip_start = "192.168.1.";
std::string uuid_start = "172:153:233:032:";


void gen_number()
{

    srand(time(NULL));

    for(int count_rnd = 0; count_rnd < MAX_COUNT; count_rnd++) {
        int r = rand() % 100 + 1;
        ip_prefix[count_rnd] = ip_start.append(std::to_string(r));
        uuid_prefix[count_rnd] = uuid_start.append(std::to_string(r));

        ip_start.clear();
        ip_start = "192.168.1.";
        uuid_start.clear();
        uuid_start = "172:153:233:032:";

    }



}

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/shared_mutex.hpp>

int count_key = 0;


void thread_initial_key(internet::security::encryption_controller<internet::security::aes_cbc>
        & enc_controller,int  start_key, std::string * ip_prefix_, std::string * uuid_prefix_)
{


    //LOG(INFO)<<"Start key IP : " << start_key;

    for(int count_key = start_key; count_key < MAX_COUNT; count_key++) {

        if(!enc_controller.find_key(ip_prefix_[count_key].c_str(), uuid_prefix_[count_key].c_str())) {
          if(count_key == 1){
            internet::security::aes_cbc *aes_cbc_ptr =
                    enc_controller.initial_key(ip_prefix_[count_key], uuid_prefix_[count_key]);

            printf("--- key hex : \t");
            hex_print((unsigned char *)&aes_cbc_ptr->key, KEY_SIZE);

            printf("--- iv hex : \t");
            hex_print((unsigned char *)&aes_cbc_ptr->iv, AES_BLOCK_SIZE);
					}
                        //count_key++;
        } else {
           LOG(INFO)<<"IP is stored in module. IP : "<<ip_prefix[count_key];
        }

				internet::security::aes_cbc *aes =
                    enc_controller.filter_key(ip_prefix_[count_key].c_str(), uuid_prefix_[count_key].c_str());

         //LOG(INFO)<<" IP New : " << aes->ip;

    }//for
}

/*
TEST(aescontroller, aes_encryption_thread)
{

    gen_number();
    //internet::security::encryption_controller<internet::security::aes_cbc>  * enc_controller = new
    //internet::security::aes_controller<message_scan::ResponseScan, internet::security::aes_cbc>();

    internet::security::aes_controller<message_scan::ResponseScan, internet::security::aes_cbc>
    aes_enc_initial;

    aes_enc_initial.initial_engine();

		for(int count_init = 0; count_init < 1; count_init++){
			LOG(INFO)<<"Store IP : " << ip_prefix[count_init]<<", UUID : "<< uuid_prefix[count_init];
			aes_enc_initial.initial_key(ip_prefix[count_init], uuid_prefix[count_init]);
		}

    internet::security::encryption_controller<internet::security::aes_cbc>   *enc_controller =
            &aes_enc_initial;

    boost::thread t1(boost::bind(&thread_initial_key, boost::ref(*enc_controller), 1,
       ip_prefix, uuid_prefix));
    boost::thread t2(boost::bind(&thread_initial_key, boost::ref(*enc_controller), 2,
       ip_prefix, uuid_prefix));
    boost::thread t3(boost::bind(&thread_initial_key, boost::ref(*enc_controller), 3,
       ip_prefix, uuid_prefix));

    t1.join();
    t2.join();
    t3.join();

}
/* */


TEST(aescontroller, encryptionstring)
{
    //http://stackoverflow.com/queto_stringns/5727646/what-is-the-length-parameter-of-aes-evp-decrypt
    //set ip, uuid

    std::string ip("127.0.0.1");
    std::string uuid("172:2343:22336:233");
	
    internet::security::encryption_controller<internet::security::aes_cbc>   *enc_controller = new
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

    bool res_filter = enc_controller->find_key(ip.c_str(), uuid.c_str());
    LOG(INFO)<<" Filter key is true : " << res_filter;

    std::string ip_false("192.168.1.2");
    std::string uuid_false("0:0:0:0");

		internet::security::aes_cbc aes_false = 
     internet::security::aes_cbc(ip_false, uuid_false);

    EXPECT_TRUE(enc_controller->process_crypto(aes_false, 
                 utils::find_key_crypto_mode) == NULL);

   internet::security::aes_cbc  aes_true = 
   	 internet::security::aes_cbc(ip.c_str(), uuid.c_str());

    EXPECT_TRUE(enc_controller->process_crypto(aes_true, 
                 utils::find_key_crypto_mode) != NULL);

}
/* */









