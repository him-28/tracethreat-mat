#include "data_structure/ac_parallel.hpp"
template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
actire_parallel<ContainerT,ContainT2>::actire_parallel()
{
    //logger
    logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
    logger = logger_ptr->get();
    logger->write_info_test("Init logger actire_parallel ...");
}

template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
bool actire_parallel<ContainerT,ContainT2>::operator=(ContainerT& container_acs)
{
    typename ContainerT::iterator iter_con_acs;
    uint64_t count_state = 0;
    typedef boost::unordered_map<SymbolT, StateT> unordered_map;
    unordered_map *map_con_acs;

    for(iter_con_acs = container_acs.begin();
            iter_con_acs != container_acs;
            ++iter_con_acs) {
        //Vector state graph
        graph_state_vec[count_state]  = std::distance(container_acs.begin(), iter_con_acs);
        map_con_acs = *iter_con_asc;

        //map
        for(unordered_map::iterator iter_map_acs  = map_con_acs.begin();
                iter_map_acs != map_con_acs.end();
                ++iter_map_acs) {
            logger->write_info_test("--actire_parallel::operator= key ",
                    boost::lexical_cast<std::string>(iter_map_acs->first));
            logger->write_info_test("--actire_parallel::operator= value ",
                    boost::lexical_cast<std::string>(iter_map_acs->second));
        }

        //count state insert to grap_state_vec
        count_state++;
    }
}

template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
set_output(std::vector<StateT, std::set<size_t> >& output_s)
{
    typename std::set<size_t> >::iterator siter_output_vec;
    //get list of position signature string found.
}


template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
set_table(std::vector<StateT>& table_s)
{
    this->table_  = table_s;
}


template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
parallel_algorithm()
{


}

