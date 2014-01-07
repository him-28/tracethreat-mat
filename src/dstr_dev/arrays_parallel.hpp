#ifndef  HNMAV_DATASTRUCTURE_ARRAYS_PARALLEL
#define  HNMAV_DATASTRUCTURE_ARRAYS_PARALLEL

/*                       Titles                                          Authors                        Date
 *- array create work items & work group                                 Chatsiri.rat								  08/02/2013
 */

//BOOST
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

// STL
#include <vector>
#include <iterator>

//INTERNAL
#include "logger/clutil_logger.hpp"
#include "data_structure/absalgorithms.hpp"
#include "data_structure/tire.hpp"

namespace hnmav_datastructure
{

    using namespace boost;

    namespace util = hnmav_util;

    template<typename TypeTire, typename NodeType>
    struct node_tire;

    template<typename NodeTireVecList, typename TireVecList>
    class absalgorithms;

    //struct datastructure_def;

    template<typename IndexType, typename TypeInput, typename TypeOutput, typename WorkLoad>
    class arrays_parallel : public absalgorithms<TypeInput, TypeOutput>
    {
        public:


            arrays_parallel();

            bool init_workload(WorkLoad& workload, TypeOutput& datatypes);

            TypeInput& find_workitems(TypeInput& type);

            bool process_workload();

            // interface of ilist
            bool add(TypeInput& input_vec);

            arrays_parallel *operator=(absalgorithms<TypeInput, TypeOutput>& abs_rsh);

            TypeInput& find(TypeInput& data) {

            }

            WorkLoad& size() {
                return *device_workload;
            }

            ~arrays_parallel() { }

        private:
            // device define types of work_items & work_groups
            WorkLoad   *device_workload;
            // type of data input sended to devices( workgroups, workitmes.)
            IndexType  *work_groups;
            IndexType  *work_items;

            TypeInput 													*tiredef;
            datastructure_def::ntire_veclist  	*ntire_vec_list;
            std::vector<node_tire<std::string, char> >   *ntire_vec;
            datastructure_def::ntire        		*ntire;

            // Handeling logger
            shared_ptr<util::clutil_logging<std::string, int> > *logger_ptr;
            util::clutil_logging<std::string, int> *logger;

            arrays_parallel<IndexType, TypeInput, TypeOutput, WorkLoad>   *arrays_parallel_;


    };

    /**
    * @brief Initial log
    */
    template<typename IndexType, typename TypeInput, typename TypeOutput, typename WorkLoad>
    arrays_parallel<IndexType, TypeInput, TypeOutput, WorkLoad>::arrays_parallel()
    {
        util::options_system& op_system = util::options_system::get_instance();
        //init logger
        logger_ptr = &util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
    }

    /**
    * @brief Class for static casting from absalgorithm to arrays_parallel
    *
    * @param abs_rsh  Absalgorithms class
    *
    * @return Object current sends to other object.
    */
    template<typename IndexType, typename TypeInput, typename TypeOutput, typename WorkLoad>
    arrays_parallel<IndexType, TypeInput, TypeOutput, WorkLoad> *
    arrays_parallel<IndexType, TypeInput, TypeOutput, WorkLoad>::operator=
    (absalgorithms<TypeInput, TypeOutput>& abs_rsh)
    {
        if(abs_rsh != NULL) {
            arrays_parallel_ = dynamic_cast<arrays_parallel<IndexType, TypeInput, TypeOutput, WorkLoad>* >(abs_rsh);
        }

        return *this;
    }

    /**
    * @brief Initialization size of workgroups & workitems.
    * Workitemes columns are sizes 50. Row sizes as column sizes.
    * [ Group A] [ Group ...End ot ASCII string]
    * [ A ] [ . ] [ . ] [ . ] [ End of worktimes are ASCII string types]
    * [ . ]
    * [ . ]
    * [ . ]
    * [ End of workitems are ASCII string type ]
    * @param workload input node of data. Input from Tire classes.
    *
    * @return False, If cannot initial size of workload
    */
    template<typename IndexType, typename TypeInput, typename TypeOutput, typename WorkLoad>
    bool arrays_parallel<IndexType, TypeInput, TypeOutput, WorkLoad>::init_workload(WorkLoad& workload, TypeOutput& ntire_veclist)
    {
        int countall_str = 0;
        logger->write_info("--- Container virus sizes ", lexical_cast<std::string>(ntire_veclist.size()));

        for(typename std::list<std::vector<node_tire<std::string, char> > >::iterator iter_tire = ntire_veclist.begin();
                iter_tire != ntire_veclist.end();
                ++iter_tire) {
            ntire_vec = &(*iter_tire);
            countall_str += ntire_vec->size();
        }

        logger->write_info("--- Size all of string search", lexical_cast<std::string>(countall_str));

        // initial workgroup , workitems after calculate
        device_workload->work_groups = workload.work_groups;
        device_workload->work_items  = workload.work_items;
        // work group , work items define
        work_groups = new IndexType[device_workload->work_groups];
        IndexType  workitems[device_workload->work_items][device_workload->work_items];

    }

    /**
    * @brief Input data node ( All Signature contains on vector )  Add to input before send to
    * proces work load.
    *
    * @param input_vec  input vector contains array of signatures.
    *
    * @return True if inputs are contain in workload.
    */
    template< typename IndexType, typename TypeInput, typename TypeOutput,typename WorkLoad>
    bool arrays_parallel<IndexType, TypeInput, TypeOutput, WorkLoad>::add(TypeInput& input_vec)
    {

    }



}


#endif /*  HNMAV_ALGORITHM_ARRAYS_PARALLEL */
