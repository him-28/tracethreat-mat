#include "exception_handler.hpp"
//#include <boost/system/error_code.hpp>
//#include <boost/cerrno.hpp>
//#include <string>


namespace  error_handler
{


//template<typename ErrorControl>
//failure_handler<ErrorControl> * failure_handler<ErrorControl>::failure_error_category = NULL;


template<typename ErrorControl>
std::string failure_handler<ErrorControl>::message(int ev) const
{
		if(ev == THREAD_NO_FILE)
			return std::string("Thread not inclued file");
		if(ev == THREAD_CANNOT_CONNECT_OCL)
			return std::string("Thread cannot connect to OCL");
		return std::string("Thread unkwon error"); 
}

template<typename ErrorControl>
boost::system::error_condition failure_handler<ErrorControl>::default_error_condition(int ev) const
{
		return ev == THREAD_NO_FILE 
				? boost::system::error_condition(boost::system::errc::io_error, boost::system::generic_category())
				:	boost::system::error_condition(ev, new failure_handler());
}

template<typename ErrorControl>
const char * failure_handler<ErrorControl>::name()const
{

		return "failture_handler";
}

template<typename ErrorControl>
const char * failure_handler<ErrorControl>::what() const throw()
{
		return "failture_handler";
}


template<typename ErrorControl>
failure_handler<ErrorControl>::failure_handler() throw() : failure<ErrorControl>()
{
//	if(failure_error_category == NULL)
//		{
//			failure_error_category = new failure_handler<ErrorControl>();
//		}
}

}

