#ifndef  HNMAV_EXCEPTION_GEN_ERROR_CODE_HPP
#define  HNMAV_EXCEPTION_GEN_ERROR_CODE_HPP

/*                       Titles                                          Authors                        Date
 * Code error of data structure                                          Chatsiri.rat                   14/01/2013
 *
 */

//STL
#include <map>

//container exception.
#define  DS_INVAILD_SIZE   0
#define  DS_INVAILD_VALUE -1

//file system exception.
#define  FILENAME_IS_NULL -2

// Algorithms internal codes.
#define  AT_INVALID_SIZE -100

namespace hnmav_exception
{

    class gen_error_code;

    static const std::map<unsigned int, const char *>::value_type  error_code_entry[] = {
        // 
        {0,  ", DS_INVAILD_SIZE" },
        {-1, ", DS_INVAILD_VALUE"},

				// Algorithm error codes
				{-100, ",  AT_INVALID_SIZE"} 
    };

    /**
    * @brief Get error code from _error_code_exception.
    */
    class gen_error_code
    {
        public:
            void set_error_cnumber(int error_cnumber);
            const std::map< unsigned int, const char *> initial_error_code();
            std::string get_msg_error();
        private:
            int error_cnumber_;
            typedef std::map< unsigned int, const char *> error_code_map;
            typedef error_code_map::value_type error_code_map_entry;
    };

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


#endif  /* HNMAV_EXCEPTION_GEN_ERROR_CODE_HPP */
