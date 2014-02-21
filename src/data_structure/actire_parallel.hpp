#ifndef DATA_STRUCTURE__ACTIRE_PARALLEL_HPP
#define DATA_STRUCTURE__ACTIRE_PARALLEL_HPP

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#include "utils/logger/clutil_logger.hpp"

namespace data_structure
{
    namespace h_util = hnmav_util;

    template<typename ContainerT>
    class iparallel
    {
        public:
            virtual	ContainerT& get_container()const = 0;
            //virtual bool operator=(ContainerT& containerT) = 0;
    };

    /**
    * @brief Ac tire parallel.
    */
    template<typename SymbolT,
             typename StateT,
             typename EdgesT,
             typename ContainerT>
    class actire_parallel : public iparallel<ContainerT>
    {
        public:
						actire_parallel();

            //bool operator=(ContainerT& container){ };

            bool prepare_graph(const ContainerT * container);

            bool set_output(std::vector<StateT, std::set<size_t> >& output_s);

            bool set_table(std::vector<StateT>& table_s);

            bool parallel_algorithm();

            ContainerT& get_container()const { };
        private:

            //Graph_
            ContainerT * container_ac_parallel;
            EdgesT     * edget_map;

						// graph :  std::vector<boost::unordered_map<SymbolT, state_t> >
            // Index of vector
            std::vector<StateT>  graph_index_vec;
            // Map
            // Symbol on map
            std::vector<SymbolT> graph_symbol_vec;
            // State of map
            std::vector<StateT>  graph_state_vec;

            //output_parallel : std::map<state_t, std::set<size_t>  
            std::vector<StateT> output_parallel;
            std::vector<StateT> output_state_vec;
            std::vector<StateT> output_setofstate_vec;

            //table_
            std::vector<StateT> table_;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}

#endif /* DATA_STRUCTURE__ACTIRE_PARALLEL_HPP */





