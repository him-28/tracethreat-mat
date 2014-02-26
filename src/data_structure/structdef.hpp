#ifndef  HNMAV_DATASTRUCTURE_DATASTRUCTURE_DEF_HPP
#define  HNMAV_DATASTRUCTURE_DATASTRUCTURE_DEF_HPP
/*                       Titles                                          Authors                        Date
 *- Define type for main 																								 Chatsiri.rat										01/03/2013

 */

#include <boost/shared_ptr.hpp>

#include <vector>
#include <list>

//#include "data_structure/tire.hpp"
//#include "data_structure/arrays_parallel.hpp"

//using namespace boost;


namespace data_structure
{

   // template<typename TireType, typename NodeType,typename OutputType, typename WorkLoad>
   // class tire;

    /**
    * @brief Set size tree keyword.
    */
    template<typename SizeType>
    struct worktypes {

        SizeType work_groups;
        SizeType work_items;

    };

		/*
    template<typename TypeTire, typename NodeType>
    struct node_tire {
        // data in node
        NodeType data;

        NodeType virus_name;

        node_tire<TypeTire, NodeType> *node_next;
        node_tire<TypeTire, NodeType> *node_prev;
        // address insert next  node
        node_tire() : data('\0') ,  node_next(NULL), node_prev(NULL) { }

    };
		*/
		
    /**
    * @brief Data structure types foring send data array_parallel class
    */
		
    namespace dstr_def
    {
        typedef worktypes<int>                              work_groupitems;
       // typedef node_tire<std::string, char>                ntire;
       // typedef std::vector<node_tire<std::string, char> >  ntire_vec;
       // typedef std::list<ntire_vec>                        ntire_veclist;
       // typedef shared_ptr<ntire_veclist>                   ntire_veclist_ptr;
       // typedef tire<std::string, char,std::list<std::vector<node_tire<std::string, char> > >,worktypes<int> > tiredef;
    }
		
		/*
		#include "filetypes/binary.hpp"
    class binary_factory;

    typedef struct scantype_def {
        typedef binary_factory binary_type;

    } scantype_def;
		*/

}


#endif /* HNMAV_DATASTRUCTURE_DATASTRUCTURE_DEF_HPP */
