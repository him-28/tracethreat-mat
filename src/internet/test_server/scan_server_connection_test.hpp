#include "internet/scan_server/scan_server.hpp"

TEST(Scan_serve, connection_server)
{

	boost::asio::io_service io_service;
	std::string ip_addr("127.0.0.1");
	const char * file_sig_path = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/database/clamav.hdb";
	internet::scan_server  scan_serv(io_service, ip_addr, 4050, file_sig_path);
	io_service.run();
	
}
