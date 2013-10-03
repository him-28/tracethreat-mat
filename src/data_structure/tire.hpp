#ifndef HNMAV_DATASTRUCTURE_TIRE_HPP
#define HNMAV_DATASTRUCTURE_TIRE_HPP

/*                       Titles                                          Authors                        Date
 *Create DFS and TireTyperies                                            Chatsiri.rat                   06/10/2012
 *Separate trie class and dfs																		         Chatsiri.rat (Step 1, Build tire)
 *                                                                                                      17/11/2012
 *Add Header for control node_tire struct                                Chatsiri.rat                   17/11/2012
 *Add_node from Scan_file.cpp to Tire.hpp Completed ( Non Debug )        Chatsiri.rat                   14/02/2012
*/

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <list>
#include <deque>
#include <memory>

#include <cstring>

#include "logger/clutil_logger.hpp"
#include "config/options_system.hpp"
#include "data_structure/absalgorithms.hpp"
#include "data_structure/arrays_parallel.hpp"
#include "exception/datastructure_exception.hpp" 
#include "structdef.hpp"

namespace hnmav_datastructure
{

    using namespace boost;
    using namespace boost::unit_test;

    namespace util          = hnmav_util;
    namespace exp_container = hnmav_exception::container;

    template<typename TireType, typename NodeType>
    struct node_tire;

    template<typename TireType, typename NodeType,typename OutputType, typename WorkLoad>
    class tire :
        public absalgorithms<datastructure_def::tiredef>
    {
        public:
            tire( );
            // Add string to node
            void add_node(TireType const& data, TireType  const& virus_name) throw( exp_container::container_exception );

            arrays_parallel<TireType, NodeType, OutputType, WorkLoad>	 process_tirekeyword(datastructure_def::ntire_vec& node);


            bool push(datastructure_def::ntire_vec& ntvec) {
                shared_ptr<OutputType> *list_ptr = &(this->absnt_list_ptr_vec.back());
           
					      if(list_ptr->get() == NULL)
                    return false;

                OutputType *nt_list = list_ptr->get();
                nt_list->push_back(ntvec);
                return true;
            }

            OutputType *get_containdata() {
                ntlist_ptr ptr = this->absnt_list_ptr_vec.back();
                OutputType *output = ptr.get();
                return output;
            }

            ~tire();
        private:
            typedef shared_ptr<OutputType> ntlist_ptr;

            node_tire<TireType, NodeType> *node;
            node_tire<TireType, NodeType> *node_temp;

            std::vector<node_tire<TireType, NodeType> > *node_root;
            std::vector<node_tire<TireType, NodeType> > *node_root_temp;

            // Handeling logger
            shared_ptr<util::clutil_logging<std::string, int> > *logger_ptr;
            util::clutil_logging<std::string, int> *logger;
    };

    template<typename TireType, typename NodeType, typename OutputType, typename WorkLoad>
    tire<TireType, NodeType, OutputType, WorkLoad>::tire()
    {
        util::options_system& op_system = util::options_system::get_instance();
        //init logger
        logger_ptr = &util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
    }

    template<typename TireType, typename NodeType, typename OutputType, typename WorkLoad>
    tire<TireType, NodeType, OutputType, WorkLoad>::~tire()
    {

    }

    /**
    * @brief Input data by signature of clamav and malware name.
    *
    * @param input_data  Signature of virus
    * @param virus_name  Malware name.
    */
    template<typename TireType, typename NodeType, typename OutputType, typename WorkLoad>
    void tire<TireType, NodeType, OutputType, WorkLoad>::add_node(TireType const& input_data,
            TireType const& virus_name) throw( exp_container::container_exception )
    {

        node = new node_tire<TireType, NodeType>();
        node_root = new  std::vector<node_tire<TireType, NodeType> >();

        const char *input_str = input_data.c_str();


        if(*input_str == '\0') {
            // Completed delete pointer of node : Faltal error
            delete node;
            delete node_temp;

            delete node_root;
            delete node_root_temp;
            throw exp_container::container_exception("String line with '\0, Input_str cloud contain strings");
        }

        while(*input_str != '\0') {
            char *input  = const_cast<char *>(input_str);
            node->data = *input;
            node_temp  = node;
            node_root->push_back(*node_temp);
            input_str = input_str + 1;
        }

        node_root_temp = node_root;

        if(!push(*node_root_temp))
            throw exp_container::container_exception("Cannot add note_root_temp to shared_ptr");
    }

}

#endif /* HNMAV_DATASTRUCTURE_TIRE_HPP */

