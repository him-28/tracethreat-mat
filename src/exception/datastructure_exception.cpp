
#include "exception/datastructure_exception.hpp"
namespace hnmav_exception
{
    namespace container
    {
        /**
               * @brief Error code defines in exception/exception_code.hpp.
               * Member funciton gets invalid code of 0 - 10
               *
               * @param error_cnumber  Define error code.
               *
               * @return DS_INVAILD_XXX ( XXX means size, value ,.etc ) return code meaning.
               */

        std::string error_code_entry::get_error_cnumber(int error_cnumber)
        {
            g_error_code.set_error_cnumber(error_cnumber);
            return g_error_code.get_msg_error();
        }

    }

}
