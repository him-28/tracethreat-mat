
#include "exception/system_exception.hpp"

namespace util_opt = hnmav_util;

class SystemExceptionEnvironment : public testing::Environment
{
    public:
        explicit SystemExceptionEnvironment(int argc,char **argv) {

        }
};


class SystemExceptionTest : public ::testing::Test
{

    protected:
        virtual void SetUp() {
						
						//logger
						logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
						logger = logger_ptr->get();
						logger->write_info_test("Init logger for test, SystemExceptionTest");
        }

        //logger
        boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
        h_util::clutil_logging<std::string, int>    *logger;

};


TEST_F(SystemExceptionTest, file_system_test)
{
  namespace	file_system_excep  = hnmav_exception::file_system;
 
	try{
			throw file_system_excep::offset_exception("[** File system show problem **]");
	}catch(file_system_excep::offset_exception & offset_excep){
		logger->write_info("Error, file_system : ", offset_excep.message_error_file_is_null());
	}

}
