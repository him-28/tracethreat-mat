#ifndef HNMAV_EXCEPTION_SYSTEM_EXCEPTION
#define HNMAV_EXCEPTION_SYSTEM_EXCEPTION

/*                       Titles                                          Authors                        Date
 *-Base exception class for Hnuman system                                Chatsiri.rat                   18/11/2012
 *  --option system  exception                                           Chatsiri.rat                   18/11/2012
 *  --cl utility exception                                               Chatsiri.rat                   26/11/2012
 *  --Changed Base to System exception name                              Chatsiri.rat                   26/11/2012
 *  --Separate sub type
 *         system : package system all
 *         cl     : opencl all
 */

//BOOST
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

//OpenCL
#include <CL/cl.h>

// STL
#include <stdexcept>
#include <string>
#include <iostream>

#include "exception/failure_handler_code.hpp"
// Internal code
#include "exception/exception_code.hpp"

namespace hnmav_exception
{
    using namespace boost;
    using namespace error;

    //________________________ Base virtual abstract ________________________________//
    class system_exception : public std::exception,  exception
    {
        public:
            virtual const char *what() const throw() = 0;
            virtual const char *process_name() const = 0;
    };

    namespace system
    {
        //_________________________  Option system Exception _____________________________//
        class option_system_exception : public system_exception
        {
            public:

                option_system_exception(std::string type) throw();

                const char *what() const throw();

                const char *process_name() const;
                // return   [error data , default file path]
                tuple<std::string, std::string> message_error_path(); 

                std::string default_path_system();

                ~option_system_exception() throw();

            private:
                std::string type_name;
        };

    }

    namespace cl
    {
        //_________________________  CL Boostraps ___________________________________//
        class clbootstrap_exception : public system_exception
        {
            public:
                clbootstrap_exception(std::string type) throw();
                ~clbootstrap_exception() throw();
            private:
                std::string type_name;
        };

        //_________________________  CL Exception ____________________________________//
        class clutil_exception : public std::runtime_error
        {
            public:
                clutil_exception(cl_int err, const std::string& name);

                std::string add_msg_err(cl_int err, const std::string& name);
        }; 
    }

    namespace file_system
    {

        class error_code_entry
        {

            public:
                error_code_entry();
                std::string get_error_cnumber(int  error_cnumber);
            private:
                int error_cnumber_;
        };
 
        class offset_exception : public system_exception
        {
            public:
                offset_exception(std::string type_name) throw();
                const char *what() const throw();
                const char *process_name() const;
                // Data structure size
                std::string message_error_file_is_null(); 
                ~offset_exception() throw();
            private:
                std::string type_name_;
                error_code_entry  error_codetype;

        };

    }

}

#endif /* HNMAV_EXCEPTION_SYSTEM_EXCEPTION */
