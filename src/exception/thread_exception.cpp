#include "threadsyncocl/thread_exception.hpp"

namespace error
{

	template<typename ErrorController, typename ErrorCode>
	const char * failture_handler<ErrorController, ErrorCode>::name()const
	{
		return "failure_handler";
	}

	template<typename ErrorController, typename ErrorCode>
	std::string failure_handler<ErrorController, ErrorCode>::message(int ev)const
	{		
			if( ev == THREAD_NO_FILE) return std::string("Thread not file scanning");
			if( ev == THREAD_CANNOT_CONNECT_OCL) return std::string("Thread haven't ocl");
			return std::string("Thread system unknow error");
	
	}

	

}



