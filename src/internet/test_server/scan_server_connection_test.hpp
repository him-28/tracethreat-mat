#include "internet/scan_server/scan_server.hpp"

TEST(Scan_serve, connection_server)
{

	boost::asio::io_service io_service;
	std::string ip_addr("127.0.0.1");
  std::string tracethreat_ip("127.0.0.1");
  //std::string port("4050");
	const char * file_sig_path = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/database/clamav.hdb";
	utils::options_system& op_system = utils::options_system::get_instance();
	internet::scan_server  scan_serv(io_service, op_system);
	//internet::scan_server  scan_serv(io_service, ip_addr, 4050, file_sig_path, ip_addr, 8089);
	io_service.run();
	
}
