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

  
}

#endif /*  HNMAV_UTIL_CLUTIL_FORMAT_LOGGER_HPP */
