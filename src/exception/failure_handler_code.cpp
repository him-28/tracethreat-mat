#include "exception/failure_handler_code.hpp"

namespace error
{

    template<typename ErrorController, typename ErrorCode>
    const char *failure_handler<ErrorController, ErrorCode>::name()const BOOST_NOEXCEPT
    {
        return "failure_handler";
    }

    template<typename ErrorController, typename ErrorCode>
    std::string failure_handler<ErrorController, ErrorCode>::message(int ev)const
    {
				/*
        if( ev == thread_notfile->value()) return std::string("Thread not file scanning");

        if( ev == thread_cannot_connect_ocl->value()) return std::string("Thread haven't ocl");

        if( ev == file_is_null->value()) return std::string("File is not path or null");
        */
				return std::string("Error codes don't supported system.");

    }

    template<typename ErrorController, typename ErrorCode>
    std::string failure_handler<ErrorController, ErrorCode>::message_detail(int ev)const
    {
        if( ev == thread_notfile->value()) return std::string("Thread not file scanning");

        if( ev == thread_cannot_connect_ocl->value()) return std::string("Thread haven't ocl");

        if( ev == file_is_null->value()) return std::string("File is not path or null");
        
				return std::string("Error codes don't supported system.");

    }

	 // default constructor issues.
   // void failure_handler<ErrorController, ErrorCode>::failure_handler() 
	 //		: boost::system::error_category()
 
    template<typename ErrorController, typename ErrorCode>
    void failure_handler<ErrorController, ErrorCode>::failure_handler_init()     {
        // rch:- issure with create singleton
        if(this->thread_notfile == NULL || 
						this->thread_cannot_connect_ocl == NULL ||
						this->file_is_null == NULL){
        this->thread_notfile = new boost::system::error_code(THREAD_NOT_FILE,*fh);
				this->thread_cannot_connect_ocl =new  boost::system::error_code(THREAD_CANNOT_CONNECT_OCL, *fh);
        this->file_is_null   = new boost::system::error_code(FILE_IS_NULL, *fh);
       	}
    }

    template<typename ErrorController, typename ErrorCode>
    boost::system::error_condition failure_handler<ErrorController, ErrorCode>::
    default_error_condition(int ev) const BOOST_NOEXCEPT
    {
        return ev == thread_notfile->value()
                ? boost::system::error_condition(boost::system::errc::io_error,
                        boost::system::generic_category())
                : boost::system::error_condition(ev, *fh);
								

    }
    template<typename ErrorController, typename ErrorCode>
    void failure_handler<ErrorController, ErrorCode>::error_code(ErrorCode& error)const
    {


    }

    template<typename ErrorController, typename ErrorCode>
    ErrorController& failure_handler<ErrorController, ErrorCode>::error_detect()const
    {

    }

    template<typename ErrorController, typename ErrorCode>
    const char *failure_handler<ErrorController, ErrorCode>::what() const throw()
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



