#include "internet/scan_server/scan_server.hpp"

TEST(Scan_serve, connection_server)
{

	boost::asio::io_service io_service;
	internet::scan_server  scan_serv(io_service, 4050);
	io_service.run();
	
}
