#include "internet/client_fresh/updated_sign.hpp";

TEST(UpdatedSignature, Updated_Sign_Resolver_DNS)
{
		internet::updated_sign   usig;
		std::string  url_addr("database.clamav.net");
		std::string  port("80");
		usig.set_srcfile("main.cvd");
		usig.set_remotename("database.clamav.net");
		usig.set_filepath("/home/chatsiri/workspacecpp/pthread_sync_ocl/src/database/main.cvd");
		usig.repos_initial(url_addr, port);

}
