#ifndef HNMAV_DATASTRUCTURE_IALGORITHMS_HPP
#define HNMAV_DATASTRUCTURE_IALGORITHMS_HPP

/*                       Titles                                          Authors                        Date
 *- Iterface algorithms to other classes.                                Chatsiri.rat								  11/02/2013
 */

#include <boost/utility.hpp>
#include "structdef.hpp"

namespace hnmav_datastructure
{
    using namespace boost;

    template<typename InputAlgorithmType, typename OutputAlgorithmType>
    class ialgorithms
    {
        public:
            virtual OutputAlgorithmType *get_containdata() = 0; 
    };

}

#endif /*  HNMAV_DATASTRUCTURE_IALGORITHMS_HPP */

