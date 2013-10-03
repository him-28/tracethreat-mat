/*                       Titles                                          Authors                        Date
 *- Type converts to string                                               Chatsiri.rat								  13/12/2012
 *
 */                       

//BOOST
#include<boost/lexical_cast.hpp>
//STL
#include<iostream>

namespace hnmav_util
{

    namespace convert
    {

        template<typename T>
        class convert2str
        {
                convert2str(const T& input_convert);
                std::string& operator <<(const T& input_convert);
            private:
                std::string str;

        };

        /**
        * @brief convert data class instance with static class 
        *
        * @param input_convert
        */
        template<typename T>
        convert2str<T>::convert2str()
        {

        }

        /**
        * @brief
        *
        * @param input_convert  Input all data convert with lexical cast by boost::lexical_cast<std::string>(T)
        *
        * @return std::string  
        */
        template<typename T>
        std::string& convert2str<T>::operator <<(const T& input_convert)
        {
            return lexical_cast<std::string>(input_convert);
        }

    }

}
