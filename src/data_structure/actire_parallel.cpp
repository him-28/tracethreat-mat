#include "data_structure/ac_parallel.hpp"

template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT = std::vector<EdgesT> >
bool actire_parallel<ContainerT,ContainT2>::operator=(ContainerT& container_acs)
{
    typename ContainerT::iterator iter_con_acs;
    uint64_t count_state = 0;

    for(iter_con_acs = container_acs.begin();
            iter_con_acs != container_acs;
            ++iter_con_acs) {
        //Vector state graph
        graph_state_vec[count_state]  = std::distance(container_acs.begin(), iter_con_acs);
				graph_symbol_vec[count_state] = *iter_con_asc; 
        count_state++;
    }
}

template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT = std::vector<EdgesT> >
bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
set_output(std::vector<StateT, std::set<size_t> > & output_s){
		typename std::set<size_t> >::iterator siter_output_vec; 
		//get list of position signature string found.		
}


template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT = std::vector<EdgesT> >
bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
set_table(std::vector<StateT> & table_s){
			this->table_  = table_s;
}


template<typename SymbolT, typename StateT, typename EdgesT, typename ContainerT = std::vector<EdgesT> >
bool actire_parallel<SymbolT, StateT, EdgesT, ContainerT>::
parallel_algorithm(){
					

}

