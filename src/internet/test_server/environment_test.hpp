#include "utils/config/options_system.hpp"
#include "utils/logger/clutil_logger.hpp"



class SystemTestingEnvironment : public testing::Environment
{
    public:

        explicit SystemTestingEnvironment(int argc,char **argv) {
            utils::options_system& op_system = utils::options_system::get_instance();
            op_system.generic_options();
            op_system.configure_options();
            op_system.process_options(argc, argv);

            if(!op_system.default_condition())
                std::cout<<" default " <<std::endl;

            //// init logger
            boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr =
                    &utils::clutil_logging<std::string, int>::get_instance();
            //
            utils::clutil_logging<std::string, int> *logger = logger_ptr->get();
            std::string logger_main_file = op_system.get_logger_mainfile_path();
            std::string logger_settings = op_system.get_logger_settingsfile_path();
            //
            logger->add_logger(logger_main_file);
            logger->add_settings(logger_settings);
            logger->init_backend(utils::severity_level::normal);
            //logger->formatter_normal();
            //logger->init_frontend();

            logger->write_info("Init System logger start : SystemTestingEnvironment");

        }
};


