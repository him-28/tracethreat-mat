#include "exception/system_exception.hpp"


namespace hnmav_exception
{

    namespace system
    {
        //_________________________  Option system Exception _____________________________//

        void option_system_exception::option_system_exception(std::string type) throw() : type_name(type) { }

        const char   *option_system_exception::what() const throw()
        {
            std::string error_default = "Error default ";
            return  error_default.c_str();
        }

        const char *option_system_exception::process_name() const
        {
            std::string name = "Option system exception";
            return name.c_str();
        }
        // return   [error data , default file path]
        tuple<std::string, std::string> option_system_exception::message_error_path()
        {
            std::string  str_error_path = " ";

            type_name = default_path_system();
            type_name = type_name.append("/").append(type_name);

            str_error_path = "Error cannot get path from file, Path swaps to default : ";
            str_error_path = str_error_path.append(type_name);

            tuple<std::string, std::string> tuple_type =
                    make_tuple<std::string, std::string>(str_error_path, type_name);


            return tuple_type;
        }

        std::string option_system_exception::default_path_system()
        {
            filesystem::path full_path(filesystem::current_path());
            return full_path.string();
        }

        ~option_system_exception::option_system_exception() throw() { }

        namespace cl
        {
            void clbootstrap_exception::clbootstrap_exception(std::string type) throw() : type_name(type) { }
            ~clbootstrap_exception::clbootstrap_exception() throw() {}

            //_________________________  CL Exception ____________________________________//
            void clutil_exception::clutil_exception(cl_int err, const std::string& name)
                : std::runtime_error(add_msg_err(err, name)) {}

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

            void error_code_entry::error_code_entry() {  }

            std::string error_code_entry::get_error_cnumber(int error_cnumber)
            {
                failure_handler<ErrorController, int> failure_h;
                failure_h.failure_handler_init(); // work around cannot use constructor to init
                boost::system::error_code ec(error_cnumber, failure_h);

                if(error_cnumber == failure_h.file_is_null->value()) {
                    return failure_h.message_detail(ec.value());
                }

                return std::string("No error code supported");
            }

            void offset_exception::offset_exception(std::string type_name)
            throw() : type_name_(type_name)
            {

            }

            const char *offset_exception::what() const throw()
            {
                std::string error_default = "Error default, Offset Exception ";
                return  error_default.c_str();
            }

            const char *offset_exception::process_name() const
            {
                std::string name = "Offset exception";
                return name.c_str();
            }
            // Data structure size
            std::string offset_exception::message_error_file_is_null()
            {
                return type_name_.append(error_codetype.get_error_cnumber(FILE_IS_NULL));
            }

            ~offset_exception::offset_exception() throw() {}

        }

    }
}
