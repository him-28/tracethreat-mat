#ifndef HNMAV_UTIL_CLUTIL_FORMAT_LOGGER_HPP
#define HNMAV_UTIL_CLUTIL_FORMAT_LOGGER_HPP

/*                       Titles                                          Authors                        Date
 * -Add boost formatt util                                               Chatsiri.rat                   23/11/2012
 * -Add align right for printing detail                                  Chatsiri.rat                   13/12/2012
 *
 */
//BOOST
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
//STL
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <string>

namespace hnmav_util
{

    // CENTER [   XXX   ]
    // LIFT   [xxx      ]
    // RIGHT  [      xxx]

    namespace format_type
    {
        enum type { type_header = 0, type_center = 1, type_lift = 2, type_right =3, type_right_detail = 4 };
    }

    using namespace boost;

    template<typename TypeData, typename CONST = int>
    class format_logger
    {
        public:
            format_logger(TypeData str, const format_type::type&  type) : str_(str), type_(type) { };
            void adjust_header();
            void adjust_center();
            void adjust_lift();
            void adjust_right();

            void align_right_detail();

            void common_adjust(TypeData& _str, TypeData& format_str);

            std::vector<TypeData> *get_str_format();

        private:
            TypeData str_;
            format_type::type type_;
            std::vector<TypeData>  vec_string;
    };

    template<typename TypeData, typename CONST>
    void format_logger<TypeData, CONST>::common_adjust(TypeData& _str, TypeData& format_str)
    {
        format formatter(format_str.c_str());

        if(_str.size() > 78) {
            std::size_t half_size = _str.size() / 2;

            formatter % std::string(_str.begin(), _str.begin() + half_size);
            vec_string.push_back(formatter.str());

            formatter % std::string(_str.begin() + half_size, _str.end());
            vec_string.push_back(formatter.str());

        } else {
            formatter % _str;
            vec_string.push_back(formatter.str());
        }

    }

    template<typename TypeData, typename CONST>
    std::vector<TypeData> *format_logger<TypeData, CONST>::get_str_format()
    {
        try {
            switch(type_) {
            case format_type::type_center :
                adjust_center();
                break;

            case format_type::type_header :
                adjust_header();
                break;

            case format_type::type_right_detail:
                break;

            default :
                std::cout<<"Cannot format your type " <<std::endl;
            }
        } catch(std::exception& ex) {
            std::cout<<"Data input error : " << ex.what() <<std::endl;
        }

        return &vec_string;
    }

    template<typename TypeData, typename CONST>
    void format_logger<TypeData, CONST>::adjust_header()
    {
        std::string _str = lexical_cast<std::string, const std::string>(str_);
        std::string format_str = "[%-d]";

        format formatter(format_str.c_str());
        formatter % _str;
        vec_string.push_back(formatter.str());

    }


    template<typename TypeData, typename CONST>
    void format_logger<TypeData, CONST>::adjust_right()
    {
        std::string _str = lexical_cast<std::string, const std::string>(str_);
        std::string format_str = "|%-90d| %n";

        common_adjust(_str, format_str);

    }

    template<typename TypeData, typename CONST>
    void format_logger<TypeData, CONST>::adjust_center()
    {
        std::string  _str = lexical_cast<std::string, const std::string>(str_);
        std::string  format_str = "|%=90d|";

        common_adjust(_str, format_str);
    }

    /**
    * @brief align right detail. Print detail logging.
    */
    template<typename TypeData, typename CONST>
    void format_logger<TypeData, CONST>::align_right_detail()
    {
        std::string  _str = lexical_cast<std::string, const std::string>(str_);
        std::string  format_str = "--- %=90d %n";

        common_adjust(_str, format_str);
    }


}

#endif /*  HNMAV_UTIL_CLUTIL_FORMAT_LOGGER_HPP */
