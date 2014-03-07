#ifndef NETWORK_X509_CERT_HPP
#define NETWORK_X509_CERT_HPP

#include <openssl/pem.h>
#include <openssl/conf.h>

template<typename CertType>
class gen_x509_cert{
		

public:

		bool mk_cert(uint8_t bits_encryption, uint8_t serial, uint8_t days_limit)
		{
				
		};

private:
	typename CertType * cert_type;
	cert_type  cert_x509_ptr;
	typename EVP_PKEY * evp_pkey
	evp_key    evp_pkey_ptr;
};


#endif /* NETWORK_X509_CERT_HPP */
