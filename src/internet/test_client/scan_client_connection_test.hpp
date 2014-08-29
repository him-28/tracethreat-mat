#include "internet/scan_client/scan_client.hpp"

TEST(ScanClient, connection_client)
{
	std::string url_addr("127.0.0.1");
	std::string port("4050");
	internet::service::scan_client scan_client(url_addr, port);

}
