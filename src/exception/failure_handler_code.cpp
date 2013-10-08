#include "exception/failure_handler_code.hpp"

namespace error
{

	template<typename ErrorController, typename ErrorCode>
	const char * failure_handler<ErrorController, ErrorCode>::name()const
	{
		return "failure_handler";
	}

	template<typename ErrorController, typename ErrorCode>
	std::string failure_handler<ErrorController, ErrorCode>::message(int ev)const
	{		
			if( ev == thread_notfile->value()) return std::string("Thread not file scanning");
			if( ev == thread_cannot_connect_ocl->value()) return std::string("Thread haven't ocl");
			return std::string("Thread system unknow error");
	
	}

	template<typename ErrorController, typename ErrorCode>
	failure_handler<ErrorController, ErrorCode>::failure_handler() 
	{
		// rch:- issure with create singleton
		//	if(thread_notfile == NULL){
				this->thread_notfile = new boost::system::error_code(THREAD_NOT_FILE,*fh);
		//	}
	}		

	template<typename ErrorController, typename ErrorCode>
	boost::system::error_condition failure_handler<ErrorController, ErrorCode>::
			default_error_condition(int ev) const
	{
				return ev == thread_notfile->value() 
							? boost::system::error_condition(boost::system::errc::io_error,
									boost::system::generic_category())
							: boost::system::error_condition(ev, *fh);
	}
	template<typename ErrorController, typename ErrorCode>
	void failure_handler<ErrorController, ErrorCode>::error_code(ErrorCode & error)const
	{


	}
		
	template<typename ErrorController, typename ErrorCode>
	ErrorController & failure_handler<ErrorController, ErrorCode>::error_detect()const
	{

	}

	template<typename ErrorController, typename ErrorCode>
	const char * failure_handler<ErrorController, ErrorCode>::what() const throw()
	{
	//	return error_detect();
	}

	template<typename ErrorController, typename ErrorCode>
	failure_handler<ErrorController, ErrorCode>::~failure_handler() throw()
	{

	}

	template class failure_handler<ErrorController, int>;
	template class failure_handler<ErrorController, std::string>;
	template class failure_handler<ErrorController::error_category, int>;
}



