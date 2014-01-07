#ifndef HNMAV_DATASTRUCTURE_BASED_ALGORITHMS_HPP
#define HNMAV_DATASTRUCTURE_BASED_ALGORITHMS_HPP

/*                       Titles                                          Authors                        Date
 *- Abstract supported algorithms interface                              Chatsiri.rat								  13/03/2013
 */

#include "boost/shared_ptr.hpp"

#include "data_structure/tire.hpp"
#include "data_structure/ialgorithms.hpp"

namespace hnmav_datastructure
{
    using namespace boost;

    template<typename InputAlgorithmType, typename OutputAlgorithmType>
    class ialgorithm;

    /**
    * @brief Abstract algorithms type interface with ialgorithms which it's contains list of data.
    */
    template<typename NodeTireVecList, typename TireVecList = datastructure_def::ntire_veclist>
    class absalgorithms : public ialgorithms<NodeTireVecList, TireVecList >
    {
        private:
            typedef shared_ptr<NodeTireVecList>         ntlist_ptr;
            std::vector <shared_ptr<NodeTireVecList> > 	ntlist_ptr_vec;

            typedef shared_ptr<TireVecList>         absnt_list_ptr;
            NodeTireVecList                         *tire_list;
        protected:
            std::vector <shared_ptr<TireVecList> >  absnt_list_ptr_vec;
        public:
            absalgorithms();

            TireVecList *get_containdata() {

                if(ntlist_ptr_vec.size() == 0) {
                    ntlist_ptr nptr(new NodeTireVecList);
                    ntlist_ptr_vec.push_back(nptr);
                }

                shared_ptr<NodeTireVecList> tire_ptr = ntlist_ptr_vec.back();
                tire_list   =  tire_ptr.get();
                std::list<std::vector<node_tire<std::string,char> > > *list = tire_list->get_containdata();
                return  list;
            }

            virtual bool push(datastructure_def::ntire_vec& nt_vec ) = 0;

    };

    /**
    * @brief Load list contains vector of type note_tire<std::string, char>
    */
    template<typename NodeTireVecList, typename TireVecList>
    absalgorithms<NodeTireVecList, TireVecList>::absalgorithms()
    {
        absnt_list_ptr  absnt_list_ptrs(new TireVecList);
        absnt_list_ptr_vec.push_back(absnt_list_ptrs);
    }

}


#endif /*  HNMAV_DATASTRUCTURE_BASED_ALGORITHMS_HPP */



