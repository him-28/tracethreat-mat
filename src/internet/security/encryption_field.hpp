#ifndef INTERNET_SECURITY_ENCRYPTION_FIELD_HPP
#define INTERNET_SECURITY_ENCRYPTION_FIELD_HPP

#include "internet/security/aes_controller.hpp"

namespace internet
{

    namespace security
    {

        template<typename MessageType, typename EncryptType>
        class secure_field
        {

							public:

                typedef encryption_controller<MessageType> encrypt_type;

                typedef boost::shared_ptr<MessageType> MsgsPointer;

                virtual bool encryption(MsgsPointer msg, encrypt_type * enc_controller) = 0;

                virtual bool decryption(MsgsPointer msg, encrypt_type * enc_controller) = 0;

        };

				//Stub class set massage encrypted/decrypted field.
        template<typename MessageType, typename EncryptType>
        class scan_field : public secure_field<MessageType, EncryptType>
        {

            public:

                typedef encryption_controller<MessageType> encrypt_type;

                typedef boost::shared_ptr<MessageType> MsgsPointer;

                bool encryption(MsgsPointer msg, encrypt_type * enc_controller);

                bool decryption(MsgsPointer msg, encrypt_type * enc_controller);

								internet::security::aes_cbc * filter_key(const char * ip, const char * uuid);

            private:
							  encrypt_type * enc_controller_;
        };


    }//security


}//internet

#endif /* INTERNET_SECURITY_ENCRYPTION_FIELD_HPP */
