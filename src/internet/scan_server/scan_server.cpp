#define  CPU_THREAD_SIZE 1
#include "internet/scan_server/scan_server.hpp"
#include "internet/scan_server/scan_connection.hpp"



namespace internet
{

    namespace asio = boost::asio;


    class scan_server::impl
    {
        public:

            impl(asio::io_service& io_service, std::string ip_addr, unsigned port, const char * file_path):
                scan_monitor_connection(new asio::io_service::work(io_service)), // Handle Async Connection
                acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::address::from_string(ip_addr), port)),
                io_service_(io_service),
								file_sig_path(file_path)
                {

                LOG(INFO)<<" Scan server start...";

                start_accept();
							  start_thread_accept(CPU_THREAD_SIZE);	
            }

            void listen_thread() {
                io_service_.run();
            }

            void start_thread_accept(uint8_t thread_count) {
                for(int count = 0; count < thread_count; count++) {
                    scan_connection_thread.create_thread(
                       boost::bind(&scan_server::impl::listen_thread, this));
                }
            }

            void start_accept() {

                scan_connection::pointer  new_connection = 
										internet::scan_connection::create(io_service_,file_sig_path);

                acceptor.async_accept(new_connection->get_socket(),
                        boost::bind(&scan_server::impl::handle_accept,
                                this,
                                new_connection,
                                asio::placeholders::error));

                LOG(INFO)<<" Start accept client connect to server";

            }


            void handle_accept(scan_connection::pointer connection,
                    const boost::system::error_code& error) {
                if(!error) {

                    LOG(INFO)<<" Connection start...";

                    //Call to scan_connection::start() member function of scan_connection.
                    connection->start();
                    //Another client accept.
                    start_accept();
                }
            }

        private:
            //prviate all
            asio::ip::tcp::acceptor  acceptor;
            asio::io_service& io_service_;
						std::string file_sig_path;


            boost::thread_group scan_connection_thread;
            boost::scoped_ptr<asio::io_service::work> scan_monitor_connection;


    };

    scan_server::scan_server(asio::io_service& io_service, 
				std::string ip_addr, 
				unsigned port, 
				const char * file_path)
        : impl_(new scan_server::impl(io_service, ip_addr, port, file_path))
    {

    }



    scan_server::~scan_server()
    {

    }



}
