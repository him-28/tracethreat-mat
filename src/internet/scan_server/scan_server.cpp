#define  CPU_THREAD_SIZE 1
#include "internet/scan_server/scan_server.hpp"
#include "internet/scan_server/scan_connection.hpp"

//Load system with class register.
#include "internet/security/load_security.hpp"
//#include "internet/security/encryption.hpp"
//#include "internet/security/aes_controller.hpp"

//CLASS_REGISTER_IMPLEMENT_REGISTRY(aes_controller,
//        internet::security::encryption_controller<internet::security::aes_cbc>);

namespace internet
{

    namespace asio = boost::asio;

    template<typename MessageType, typename EncryptType> class aes_controller;
    template<typename EncryptType> class encryption_controller;


    class scan_server::impl
    {
        public:
            typedef boost::shared_ptr<utils::meta_sig>  msig_ptr;

						typedef internet::security::encryption_controller<internet::security::aes_cbc> encryption_type;

            typedef asio::io_service& io_service_type;

            impl(io_service_type io_service, std::string ip_addr, unsigned port, const char *file_path):
                scan_monitor_connection(new asio::io_service::work(io_service)), // Handle Async Connection
                acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::address::from_string(ip_addr), port)),
                io_service_(io_service),
                file_sig_path(file_path),
                context_(boost::asio::ssl::context::sslv23) {

                LOG(INFO)<<"Server : Scan server start...";

                LOG(INFO)<<"Server : Initial context ssl...";

                context_.set_options(asio::ssl::context::default_workarounds
                        | asio::ssl::context::no_sslv2
                        | asio::ssl::context::single_dh_use);
                context_.set_password_callback(boost::bind(&scan_server::impl::get_password, this));
                context_.use_certificate_chain_file("server.pem");
                context_.use_private_key_file("server.pem", asio::ssl::context::pem);
                context_.use_tmp_dh_file("dh512.pem");

                LOG(INFO)<<"Server : Initial context success.";

                LOG(INFO)<<"Server : Load scanning engine...";

                //deploy scanning engine, load database to SHM.
                if(!deploy_scan_engine())
                    LOG(INFO)<<"Server : deploy scan engine compeleted.";

                //load cryto and network security engine.
                if(!load_system_engine())
                    LOG(INFO)<<"Server : Load system fail, Not completed steps to load component.";


                start_accept();
                start_thread_accept(CPU_THREAD_SIZE);
            }

            std::string get_password() const {
                return "test";
            }

            void listen_thread() {
                //boost::recursive_mutex::scoped_lock lock(res_mux);
                io_service_.run();
            }

            void start_thread_accept(uint8_t thread_count) {
                for(int count = 0; count < thread_count; count++) {
                    scan_connection_thread.create_thread(
                            boost::bind(&scan_server::impl::listen_thread, this));
                }

                scan_connection_thread.join_all();

            }

            void start_accept() {

                LOG(INFO)<<"Server : New start_accept()";

                scan_connection::pointer  new_connection =
                        internet::scan_connection::create(io_service_, 
															scan_file, 
															context_,
                              enc_controller_);

                acceptor.async_accept(new_connection->get_socket(),
                        boost::bind(&scan_server::impl::handle_accept,
                                this,
                                new_connection,
                                asio::placeholders::error));

                LOG(INFO)<<"Server : Start accept client connect to server";
                LOG(INFO)<<"Server : thread id : " <<boost::this_thread::get_id();
            }


            void handle_accept(scan_connection::pointer connection,
                    const boost::system::error_code& error) {
                if(!error) {

                    LOG(INFO)<<" Server : Connection start...";

                    //Call to scan_connection::start() member function of scan_connection.
                    connection->start();
                    //Another client accept.
                    start_accept();
                } else {
                    LOG(INFO)<<"Server : Error in handle_accept : "<< error.message();
                }
            }

					  //Default load another system.
            bool load_system_engine() {
                //Logging-monitoring system.


                //Crypto and Network Security
                LOG(INFO)<<"Server : Load security module.";
                internet::security::get_encryption().reset(
                        internet::security::create_encryption());

                if(internet::security::get_encryption().get() == NULL) {
                    LOG(INFO)<<"System cannot initial encryption engine";
                    return false;
                }

								enc_controller_ = internet::security::get_encryption()->get_encryption();
								if(enc_controller_ == NULL){
									LOG(INFO)<<"Encryption controller cannot intial";
								}
										
                //Database


                //Scan engine.
								return true;
            }

            //Deploy scanning system and load object before call scan_file member function.
            bool deploy_scan_engine() {

                LOG(INFO)<<"Server : Start deploy scan engine.";


                sig_parse = new parser::hdb_sig();

                msig_parse_vec = new std::vector<parser::meta_sigparse *>();

                sig_parse->parse_file_sig(file_sig_path.c_str(), msig_parse_vec);

                LOG(INFO)<<"Server : Signature Database path : " << file_sig_path;

                LOG(INFO)<<"Server : Signature size : "<<msig_parse_vec->size();

                if(msig_parse_vec->empty()) {
                    LOG(INFO)<<"Server : Signature DB deploy system is not signature.";
                    return false;
                }

                //Load PE Scanning engine.
                LOG(INFO)<<"Server : PE Scanning Engine start.";

                scan_file = new policy::scan_pe_internet_controller<struct MAPPED_FILE_PE>();

                std::vector<utils::meta_sig *> msig_vec;

                std::vector<parser::meta_sigparse *>::iterator iter_msig_parse;

                int count_sig = 0;

                for(iter_msig_parse = msig_parse_vec->begin();
                        iter_msig_parse != msig_parse_vec->end();
                        ++iter_msig_parse) {

                    parser::meta_sigparse   *msig_parse = *iter_msig_parse;

                    LOG(INFO)<<" Server : Signature : " << msig_parse->md5;
                    LOG(INFO)<<" Server : Virname   : " << msig_parse->virname;

                    LOG(INFO)<<"[Test] PE File test only, Please change type when to production!";

                    msig_vec.push_back(new struct utils::meta_sig);
                    utils::meta_sig *msig = msig_vec[count_sig];

                    msig->sig = new char(msig_parse->md5.length());
                    msig->sig = msig_parse->md5.c_str();

                    msig->sig_type = utils::pe_file;

                    msig->sig_size = msig_parse->md5.length();
                    msig->virname = new char(msig_parse->virname.length());
                    msig->virname = msig_parse->virname.c_str();

                    count_sig++;
                }

                if(!scan_file->load_database(&msig_vec, "shm-pe")) {
                    LOG(INFO)<<" PE Scanning cannot load_database";
                    return false;
                }

                if(!scan_file->load_engine(utils::pe_file)) {
                    LOG(INFO)<<"  PE Scanning cannot load_engine";
                    return false;
                }

                LOG(INFO)<<"Server : Deploy system success!";
                return true;
            }

            ~impl() {
                //scan_connection_thread.interrupt();
            }

        private:
            //prviate all
            asio::ip::tcp::acceptor  acceptor;
            //Context call ssl
            asio::ssl::context context_;
            //Service of socket.
            io_service_type  io_service_; // & io_service_
            //File signature path
            std::string file_sig_path;

            boost::thread_group scan_connection_thread;
            boost::scoped_ptr<asio::io_service::work> scan_monitor_connection;

            parser::sig_parser *sig_parse;

            std::vector<msig_ptr>  msig_ptr_vec;

            std::vector<parser::meta_sigparse *>   *msig_parse_vec;

            policy::scan_internet_controller<struct MAPPED_FILE_PE> *scan_file;

						encryption_type * enc_controller_; 

            //handle thread
            //mutable boost::recursive_mutex res_mux;


    };

    scan_server::scan_server(io_service_type io_service,
            std::string ip_addr,
            unsigned port,
            const char *file_path)
        : impl_(new scan_server::impl(io_service, ip_addr, port, file_path))
    {

    }



    scan_server::~scan_server()
    {

    }



}
