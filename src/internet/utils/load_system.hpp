#ifndef INTERNET_SECURITY_HPP
#define INTERNET_SECURITY_HPP

//#include "internet/utils/load_system_register.hpp"
#include "internet/security/aes_controller.hpp"

namespace internet
{

//    namespace utils
//    {
		 // 	using namespace internet::security;

        namespace security
        {

					 CLASS_REGISTER_IMPLEMENT_REGISTRY(encryption_register, encryption);

            boost::scoped_ptr<encryption>& get_encryption()
            {
                static boost::scoped_ptr<encryption> encryption;
                return encryption;
            }//get_encryption

            encryption *create_encryption()
            {
                encryption *encrypt = NULL;

                //Set flag encryption controller in "encryption_controller"
                encrypt = CREATE_ENCRYPTION("aes_encryption");

                if(encrypt == NULL) {
                    LOG(INFO)<<" Cannot create mapper : encryption_controller";
                }

                return encrypt;
            }//create_encryption


        }//load_system

//    }//utils

}//internet


#endif /* INTERNET_LOAD_SYSTEM_HPP */
