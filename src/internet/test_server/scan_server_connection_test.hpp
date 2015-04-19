#include "internet/scan_server/scan_server.hpp"

TEST(Scan_serve, connection_server)
{

	boost::asio::io_service io_service;
	utils::options_system& op_system = utils::options_system::get_instance();
	internet::scan_server  scan_serv(io_service, op_system);
	io_service.run();
	
}
