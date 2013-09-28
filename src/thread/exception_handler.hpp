#ifndef ERROR_HANDLER_EXCEPTION_HANDLER_HPP
#define ERROR_HANDLER_EXCEPTION_HANDLER_HPP

#include <boost/system/error_code.hpp>
#include <boost/cerrno.hpp>
#include <string>

namespace error_handler
{

    template<typename ErrorControl>
    class failure
    {
        public:
            virtual void error_code(ErrorControl& error) const = 0;
            virtual ErrorControl error_detect() const  = 0;
    };

    template<typename ErrorControl>
    class failure_handler : public failure<ErrorControl>, 
														public boost::system::error_category,
														public std::exception
    {
        public:
            // Error code
            enum error { THREAD_NO_FILE = 100, THREAD_CANNOT_CONNECT_OCL = 110};
            // Create instance.
            failure_handler()throw();

            const boost::system::error_category& get_failure_handler();
            std::string message(int ev)const;
						boost::system::error_condition default_error_condition(int ev) const;
						const char * name()const;
	
						const char * what()const throw();
						
            void error_code(ErrorControl& error)const{ std::string("test"); }
            ErrorControl error_detect() const{ std::string("test2"); return what_str_; }

						~failure_handler()throw() { }
        private:
            ErrorControl what_str_;
            //static const  failure_handler<ErrorControl> *failure_error_category;
    };

}

#endif /* ERROR_HANDLER_EXCEPTION_HANDLER_HPP */
