#include "utils/config/options_system.hpp"
#include "utils/logger/clutil_logger.hpp"

class object_controller
{

    public:

        object_controller(utils::options_system& ops): ops_(ops) {

        }

        void read_commandline(int argc, char **argv) {

            ops_.generic_options();
            ops_.configure_options();
            ops_.process_options(argc, argv);
        }

        std::string get_certificate_path()const {
            return ops_.get_certssl_path();
        }


    private:

        utils::options_system& ops_;

};




TEST(SystemTesting, OptionDefine)
{


    utils::options_system& op_system = utils::options_system::get_instance();
    object_controller obj_conl(op_system);
    //int argc = 3;
    //"-e /home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/test_server/ca.pem";
		//obj_conl.read_commandline(argc, &argv);
		EXPECT_EQ("/home/chatsiri/workspacecpp/pthread_sync_ocl/src/internet/test_server/ca.pem", 
              obj_conl.get_certificate_path());

}
