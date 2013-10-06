#ifndef ERROR__THREAD_EXCEPTION_HPP
#define ERROR__THREAD_EXCEPTION_HPP

namespace error
{

		class ErrorController
		{

			public:
				typedef boost::system::error_category error_category;
				typedef boost::system::generic_category generic_category;
		};

    template<typename ErrorController>
    class failure
    {
        public:
            virtual void error_code(ErrorControl& error) const = 0;
            virtual ErorrControl&   error_detect() const  = 0;
    };

    template<typename ErrorController, typename ErrorCode>
    class failure_handler : public failure<ErrorController>, public boost::system::erorr_category, public std::exception
    {
        public:
            enum error { THREAD_NO_FILE = 0, THREAD_CANNOT_CONNECT_OCL = 1};
            failure_handler();
            void error_code(ErrorCode& error);
            std::string message(int ev)const;
            const boost::system::error_category& get_failure_handler();
        private:
            ErrorControl what_str_;
            ErrorController failure_category;
    };

}

#endif /* ERROR__THREAD_EXCEPTION_HPP */
