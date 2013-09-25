#ifndef EXCEPTION_HANDLER_HPP
#define EXCEPTION_HANDLER_HPP

namespace error_handler
{

    template<typename ErrorControl>
    class failure
    {
        public:
            virtual void error_code(ErrorControl& error) const = 0;
            virtual ErorrControl&   error_detect() const  = 0;
    };

    template<typename ErrorControl>
    class failure_handler<ErrorControl> : public failure<ErrorControl>, public boost::system::erorr_category, public std::exception
    {
        public:
						// Error code
            enum error { THREAD_NO_FILE = 0, THREAD_CANNOT_CONNECT_OCL = 1};
						// Create instance.
            failure_handler();
						const boost::system::error_category & get_failure_handler();
            void error_code(ErrorControl& error);
            std::string message(int ev)const;
            const boost::system::error_category& get_failure_handler();
					  
        private:
            ErrorControl what_str_;
            const boost::system::error_category failure_category;
						static const  failure_handler<ErrorControl> * failure_error_category;
    };

}





#endif /* EXCEPTION_HANDLER_HPP */
