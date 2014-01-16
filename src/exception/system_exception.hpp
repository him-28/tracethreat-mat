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

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

//OpenCL
#include <CL/cl.h>

// STL
#include <stdexcept>
#include <string>
#include <iostream>

#include "exception/failure_handler_code.hpp"

namespace hnmav_exception
{
    using namespace boost;
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

                option_system_exception(std::string type) throw() : type_name(type) { }

                const char *what() const throw() {
                    std::string error_default = "Error default ";
                    return  error_default.c_str();
                }

                const char *process_name() const {
                    std::string name = "Option system exception";
                    return name.c_str();
                }
                // return   [error data , default file path]
                tuple<std::string, std::string> message_error_path() {
                    std::string  str_error_path = " ";

                    type_name = default_path_system();
                    type_name = type_name.append("/").append(type_name);

                    str_error_path = "Error cannot get path from file, Path swaps to default : ";
                    str_error_path = str_error_path.append(type_name);

                    tuple<std::string, std::string> tuple_type = make_tuple<std::string, std::string>(str_error_path, type_name);


                    return tuple_type;
                }

                std::string default_path_system() {
                    filesystem::path full_path(filesystem::current_path());
                    return full_path.string();
                }

                ~option_system_exception() throw() { }

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
                clbootstrap_exception(std::string type) throw() : type_name(type) { }
                ~clbootstrap_exception() throw() {}
            private:
                std::string type_name;
        };

        //_________________________  CL Exception ____________________________________//
        class clutil_exception : public std::runtime_error
        {
            public:
                clutil_exception(cl_int err, const std::string& name)
                    : std::runtime_error(add_msg_err(err, name)) {}

                std::string add_msg_err(cl_int err, const std::string& name);
        };

        std::string clutil_exception::add_msg_err(cl_int err, const std::string& name)
        {
            std::string error_msg("Error : ");
            error_msg.append(" ( ");
            error_msg.append(boost::lexical_cast<std::string>(err));
            error_msg.append(" ) ");
            return error_msg;
        }

    }

    namespace file_system
    {

        class error_code_entry
        {

            public:
                error_code_entry() {  }
                std::string get_error_cnumber(int  error_cnumber);
            private:
                int error_cnumber_;
                failure_handler<ErrorController, int> failure_h;
        };

        std::string error_code_entry::get_error_cnumber(int error_cnumber)
        {
            if(error_cnumber == FILE_IS_NULL)
                return failure_h.message(failure.message(failure_h.file_is_null->value()));
        }

        class offset_exception : public system_exception
        {
            public:
                offset_exception(std::string type_name) throw() : type_name_(type_name) { }

                const char *what() const throw() {
                    std::string error_default = "Error default, Offset Exception ";
                    return  error_default.c_str();
                }

                const char *process_name() const {
                    std::string name = "Offset exception";
                    return name.c_str();
                }
                // Data structure size
                std::string message_error_size_initial() {
                    return type_name_.append(error_codetype.get_error_cnumber( DS_INVAILD_SIZE  ));
                }

                std::string message_at_error_size() {
                    return type_name_.append(error_codetype.get_error_cnumber( AT_INVALID_SIZE ));
                }

                ~offset_exception() throw() {}
            private:
                std::string type_name_;
                error_code_entry  error_codetype;
        };

    }

}

#endif /* HNMAV_EXCEPTION_SYSTEM_EXCEPTION */
