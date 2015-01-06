#ifndef INTERNET_SECURITY_ENCRYPTION_HPP
#define INTERNET_SECURITY_ENCRYPTION_HPP

#include "utils/base/class_register.hpp"
#include "internet/security/aes_controller.hpp"

namespace internet
{

    namespace security
    {
				template<typename EncryptType> class encryption_controller;
				struct aes_cbc;

        class encryption
        {

            public:
                //encryption(){ }
                //virtual bool initial_engine() = 0;

                virtual internet::security::aes_cbc *initial_key(std::string ip, std::string uuid) = 0;

                virtual bool filter_key(const char *ip, const char *uuid) = 0;

                virtual bool encryption_msgs(const char *msg, int msg_length) = 0;

                virtual bool decryption_msgs(const char *msg, int msg_length)=  0;

               // template<typename EncryptType = internet::security::aes_cbc>
               // internet::security::encryption_controller<EncryptType>& get_encryption();
								virtual internet::security::encryption_controller<internet::security::aes_cbc> & 
                 get_encryption() = 0;
						
                virtual ~encryption() { }
        };

        //System load crypto controller
        CLASS_REGISTER_DEFINE_REGISTRY(encryption_register, encryption);

        //Define macro of CLASS_REGISTER_OBJECT_CREATOR, CLASS_REGISTER_CREATE_OBJECT
#define REGISTER_ENCRYPTION(encryption_name) \
                      CLASS_REGISTER_OBJECT_CREATOR( \
                      encryption_register, \
                      encryption, \
                      #encryption_name, \
                      encryption_name) \
 
#define CREATE_ENCRYPTION(encryption_name_as_string) \
                      CLASS_REGISTER_CREATE_OBJECT(encryption_register, \
													encryption_name_as_string);



    }//security

}//internet



#endif /* INTERNET_SECURITY_ENCRYPTION_HPP */
