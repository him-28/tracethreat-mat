#include "exception/exception_code.hpp"


namespace hnmav_exception
{
  
 
    /**
    * @brief Add error code to std::map
    *
    * @return return map value to get_msg_error() member function
    */
    const std::map<unsigned int, const char *>  gen_error_code::initial_error_code()
    {
        const error_code_map error_cmap( begin(error_code_entry), end(error_code_entry) );
        return  error_cmap;
    }

    /**
    * @brief Receive error code from member function in order to get error number.
    *
    * @param error_cnumber  Number of error.
    */
    void gen_error_code::set_error_cnumber(int error_cnumber)
    {
        this->error_cnumber_ = error_cnumber;
    }

    /**
    * @brief Send error code to number function called it.
    *
    * @return Meaning of error.
    */
    std::string gen_error_code::get_msg_error()
    {
        const std::map<unsigned int, const char *>  error_cmap  =  initial_error_code();
        const std::map<unsigned int, const char *>::const_iterator it = error_cmap.find(error_cnumber_);

        if(it == error_cmap.end())
            return "UNKNOW ERROR";

        return it->second;
    }


}


