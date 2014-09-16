#include "internet/scan_server/scan_server.hpp"

TEST(Scan_serve, connection_server)
{

	boost::asio::io_service io_service;
	std::string ip_addr("127.0.0.1");
	internet::scan_server  scan_serv(io_service, ip_addr, 4050);
	io_service.run();
	
}
