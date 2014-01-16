#ifndef ERROR__THREAD_EXCEPTION_HPP
#define ERROR__THREAD_EXCEPTION_HPP

#include <boost/system/error_code.hpp>
#include <boost/cerrno.hpp>

#include <exception>

namespace error
{
    enum thread_systme_error {
        THREAD_NOT_FILE = 110,
        THREAD_CANNOT_CONNECT_OCL = 120
    };

		enum file_system_error{
				FILE_IS_NULL = 210,
				FLIE_NO_PATH = 220
		};

    class ErrorController
    {

        public:
            typedef boost::system::error_category error_category;
    };

    template<typename ErrorController, typename ErrorCode>
    class failure
    {
        public:
            virtual void error_code(ErrorCode& error) const = 0;
            virtual ErrorController&   error_detect() const  = 0;
    };

    template<typename ErrorController, typename ErrorCode>
    class failure_handler :
        public failure<ErrorController, ErrorCode>,
        public boost::system::error_category,
        public std::exception
    {
        public:

            void failure_handler_init();

            void error_code(ErrorCode& error)const;
            ErrorController& error_detect()const;

            const char *name()const;

            std::string message(int ev)const;

            std::string message_detail(int ev)const;

            boost::system::error_condition default_error_condition(int ev) const;

            const char *what() const throw();
            //	~exception() throw(){ }
            ~failure_handler() throw();

						// thread error code
            const boost::system::error_code *thread_notfile;
            const boost::system::error_code *thread_cannot_connect_ocl;
						// file system error code
						const boost::system::error_code *file_is_null;

        private:
            //ErrorController failure_category;
            failure_handler<ErrorController, ErrorCode> *fh;
    };

}

#endif /* ERROR__THREAD_EXCEPTION_HPP */
