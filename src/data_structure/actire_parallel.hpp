#ifndef DATA_STRUCTURE__ACTIRE_PARALLEL_HPP
#define DATA_STRUCTURE__ACTIRE_PARALLEL_HPP

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#include "utils/logger/clutil_logger.hpp"

namespace data_structure
{
    namespace h_util = hnmav_util;

    template<typename SymbolT, typename StateT>
    class iparallel
    {
        public:
            //typedef  std::vector<SymbolT>  symbol_vec;
            //typedef  std::vector<StateT>   state_vec;

            /* virtual	boost::tuple<std::vector<SymbolT>,
                     std::vector<StateT> > * get_container()const = 0; */
            virtual	const std::map<std::vector<SymbolT>, std::vector<StateT> > *get_container() const = 0;
						virtual boost::shared_ptr<std::vector<SymbolT> > get_symbol_shared_ptr() = 0;
            virtual boost::shared_ptr<std::vector<StateT> >  get_state_shared_ptr() = 0;
    };

    /**
    * @brief Ac tire parallel.
    */
    template<typename SymbolT,
             typename StateT,
             typename EdgesT,
             typename ContainerT>
    class actire_parallel : public iparallel<SymbolT, StateT>
    {
        public:
            typedef  boost::tuple<std::vector<StateT>,
                     std::vector<StateT> > tuple_vec;
            typedef  std::vector<SymbolT>  symbol_vec;
            typedef  std::vector<StateT>   state_vec;

            actire_parallel() : symbol_shared_ptr_vec(boost::make_shared<symbol_vec>()),
                state_shared_ptr_vec(boost::make_shared<state_vec>()) {

                //logger
                logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
                logger = logger_ptr->get();
                logger->write_info_test("Init logger actire_parallel ...");


            }

            //bool operator=(ContainerT& container){ };

            bool prepare_graph(const ContainerT *container);

            bool set_output(std::vector<StateT, std::set<size_t> >& output_s);

            bool set_table(std::vector<StateT>& table_s);

            bool parallel_algorithm();

            /*
            boost::tuple<std::vector<SymbolT>,
            std::vector<StateT> > * get_container()const {
            if(graph_symbol_vec.size() == 0 || graph_state_vec.size() == 0) {
            logger->write_info("Symbol vector or State vector not data");
            }

            boost::tuple<std::vector<SymbolT>,
            std::vector<StateT> >  symbol_state_tuple = boost::make_tuple(graph_symbol_vec,
                  graph_state_vec);
            return  &symbol_state_tuple;

            }*/

						//TO:DO  Map key by struct MAPPED_FILE_* such as PE, EXE
            virtual const std::map<std::vector<SymbolT>, std::vector<StateT> > * get_container() const{
                return &map_symbol_state;
            }


            virtual boost::shared_ptr<symbol_vec> get_symbol_shared_ptr() {
                return symbol_shared_ptr_vec;
            }

            virtual boost::shared_ptr<state_vec> get_state_shared_ptr() {
                return state_shared_ptr_vec;
            }

        private:

            //Graph_
            ContainerT *container_ac_parallel;
            EdgesT      *edget_map;

            // graph :  std::vector<boost::unordered_map<SymbolT, state_t> >
            // Index of vector
            std::vector<StateT>  graph_index_vec;
            // Map
            // Symbol on map
            std::vector<SymbolT> graph_symbol_vec;
            // State of map
            std::vector<StateT>  graph_state_vec;

            boost::shared_ptr<symbol_vec>    symbol_shared_ptr_vec;
            boost::shared_ptr<state_vec>     state_shared_ptr_vec;

            boost::tuple<std::vector<SymbolT>,
                  std::vector<StateT> >  symbol_state_tuple;

            std::map<std::vector<SymbolT>, std::vector<StateT> > map_symbol_state;

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





