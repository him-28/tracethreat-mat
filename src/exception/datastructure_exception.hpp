#ifndef HNMAV_EXCEPTION_DATASTRUCTURE_EXCEPTION_HPP
#define HNMAV_EXCEPTION_DATASTRUCTURE_EXCEPTION_HPP

/*                       Titles                                          Authors                        Date
 *- Add vector exception type																						 Chatsiri.rat                   14/01/2013
 *- Add First Algorihms tire defines error code.                         Chatsiri.rat                   11/02/2013
 *- Algorithm message_at_error_size is error size of algorithms.         Chatsiri.rat                   12/02/2013.
 */

//BOOST
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

// Internal code
#include "exception/exception_code.hpp"

// STL
#include <stdexcept>
#include <string>
#include <iostream>
#include <map>


namespace hnmav_exception
{
    using namespace boost;

		class error_code_entry;

    class gen_error_code;

    
    //________________________ Base virtual abstract ________________________________//
    class datastructure_exception : public std::exception,  exception
    {
        public:
            virtual const char *what() const throw() = 0;
            virtual const char *process_name() const = 0;
    };

    namespace container
    {
        //________________________ Error code ________________________________//
        /**
        * @brief Get error code of data structure.
        */
        class error_code_entry
        {

            public:
                error_code_entry() {  }
                std::string get_error_cnumber(int  error_cnumber);
            private:
                int error_cnumber_;
                gen_error_code g_error_code;
        };


        //________________________ Exception of container type ________________________________//
        /**
        * @brief  Container types group of vector, list internal datastructure.
        * 				-- Container
        * 				 --- Vector_Buffer
        * 				 --- List_Buffer
        * 				 --- tire_Buffer
        */
        class container_exception    : public datastructure_exception
        {

            public:
                container_exception(std::string type_name) throw() : type_name_(type_name) { }

                const char *what() const throw() {
                    std::string error_default = "Error default, Container Exception ";
                    return  error_default.c_str();
                }

                const char *process_name() const {
                    std::string name = "Container exception";
                    return name.c_str();
                }
                // Data structure size
                
								std::string message_error_size_initial() {
                    return std::string("error"); 
										//type_name_.append(error_codetype.get_error_cnumber( DS_INVAILD_SIZE  ));
                }

                std::string message_at_error_size() {
                    return type_name_.append(error_codetype.get_error_cnumber( AT_INVALID_SIZE ));
                }

                ~container_exception() throw() {}
            private:
                std::string type_name_;
                error_code_entry  error_codetype;
        };

    }

}


#endif /* HNMAV_EXCEPTION_DATASTRUCTURE_EXCEPTION_HPP */
