
#include "data_structure/actire_parallel.hpp"

namespace data_structure
{
		/*		
    template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
    actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::actire_parallel()
    { 
				
        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        logger->write_info_test("Init logger actire_parallel ...");
    }
		*/

    template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
    bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
    prepare_graph(const ContainerT * container_acs)
    {
        typename ContainerT::const_iterator iter_con_acs;
        //uint64_t count_state = 0;
        typedef boost::unordered_map<SymbolT, StateT> unordered_map;
        unordered_map map_con_acs;

        for(iter_con_acs = container_acs->begin();
                iter_con_acs != container_acs->end();
                ++iter_con_acs) {
            //Vector state graph
            graph_state_vec.push_back(std::distance(container_acs->begin(), iter_con_acs));
            map_con_acs = *iter_con_acs;

            //map
            for(typename unordered_map::iterator iter_map_acs  = map_con_acs.begin();
                    iter_map_acs != map_con_acs.end();
                    ++iter_map_acs) {
                logger->write_info_test("--actire_parallel::operator= key ",
                        boost::lexical_cast<std::string>(iter_map_acs->first));
								//key  : Symbol
								//graph_symbol_vec.push_back(iter_map_acs->first);
								symbol_shared_ptr_vec->push_back(iter_map_acs->first);

                logger->write_info_test("--actire_parallel::operator= value ",
                        boost::lexical_cast<std::string>(iter_map_acs->second));
								//value : State
								//graph_state_vec.push_back(iter_map_acs->second);
							  state_shared_ptr_vec->push_back(iter_map_acs->second);
            }
						logger->write_info_test("------- End state_push_back --------");

            //count state insert to grap_state_vec
            //count_state++;
        }
    }

    template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT>
    bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
    set_output(std::vector<StateT, std::set<size_t> >& output_s)
    {
        typename std::set<size_t>::iterator siter_output_vec;
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

		template class actire_parallel<char, 
																	size_t,
																	boost::unordered_map<char, size_t>, 
																	std::vector<boost::unordered_map<char, size_t> > >;
		
}

