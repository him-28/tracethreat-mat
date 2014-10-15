#ifndef  HNMAV_DATASTRUCTURE_DATASTRUCTURE_DEF_HPP
#define  HNMAV_DATASTRUCTURE_DATASTRUCTURE_DEF_HPP
/*                       Titles                                          Authors                        Date
 *- Define type for main 																								 Chatsiri.rat										01/03/2013

 */

#include <boost/shared_ptr.hpp>

#include <vector>
#include <list>

namespace data_structure
{


    /**
    * @brief Set size tree keyword.
    */
    template<typename SizeType>
    struct worktypes {

        SizeType work_groups;
        SizeType work_items;

    };


    /**
    * @brief Data structure types foring send data array_parallel class
    */

    namespace dstr_def
    {
        typedef worktypes<int>                              work_groupitems;
    }

}


#endif /* HNMAV_DATASTRUCTURE_DATASTRUCTURE_DEF_HPP */
