#ifndef HNMAV_KERNEL_CLUTIL_MEMORY_HPP
#define HNMAV_KERNEL_CLUTIL_MEMORY_HPP

/*                       Titles                                          Authors                        Date
 *-Add utility for managment memory                                      Chatsiri.rat										26/08/2012
 *-Move utiltity class to PIMP idom                                      Chatsiri.rat										27/08/2012
 *-Add interface class                                                   Chatsiri.rat										27/08/2012
 *-API concept using Proxy class	clutil_memory	                         Chatsiri.rat										01/09/2012
 */

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

//OPENCL
#include "CL/cl.h"

//STL
#include <string.h>
#include <stdlib.h>

//INTERNAL
#include "ocl/utils/base_clutil.hpp"
// Tire initial class for insert to OpenCL
//#include "data_structure/tire.hpp"
// Exceptions & Logging
#include "utils/logger/clutil_logger.hpp"
//- Data structure
#include "data_structure/vector.hpp"
// Array parallel
#include "data_structure/actire_parallel.hpp"
// Define type
#include "data_structure/structdef.hpp"
// abstract algorithms
//#include "data_structure/absalgorithms.hpp"
#include <boost/unordered_map.hpp>

#include "exception/system_exception.hpp"
#include "exception/datastructure_exception.hpp"

namespace hnmav_kernel
{

    //using namespace boost;
    namespace dstr   	= data_structure;
    namespace util   	= hnmav_util;
    namespace except  = hnmav_exception;

    //  memory internal initial, write Real Object here.
    template<typename WorkTypes,
             typename ContainerT = std::vector<size_t> >
     class memory : public base_clutil
    {
        public:

            memory();

            bool  cl_create_buffer(WorkTypes& worktype_loads,
										std::vector<char>   & symbol_vec_ptr,
										std::vector<size_t> & state_vec_ptr,
                    std::vector<char>& binary_vec_ptr);

            bool  cl_build_node_buffer_object() throw(std::runtime_error);

            bool  cl_create_subbuffer();

            bool  cl_check_buffer_size();

            bool  cl_release_memory();

            // Interface class
            std::vector<boost::shared_ptr<platdevices_info> >&    get_platdevices_info_vec() {
                return platdevices_shared_ptr;
            }

            void set_platdevices_info_vec(std::vector<boost::shared_ptr<platdevices_info> >& ptr_info) {
                platdevices_shared_ptr = ptr_info;
            }

            platdevices_info *get_platdevices_data() {
                boost::shared_ptr<platdevices_info>   plat_shared_ptr = platdevices_shared_ptr.back();
                platdevices_info *platdevices =  plat_shared_ptr.get();
                return platdevices;
            }


        private:

            std::size_t buffer_elements;
            std::vector<boost::shared_ptr<platdevices_info> >  platdevices_shared_ptr;
            // cl define
            platdevices_info *platdevices;

            boost::shared_ptr<util::clutil_logging<std::string, int> >    *logger_ptr;
            util::clutil_logging<std::string, int>    *logger;

            // input open cl
            std::vector<char> *symbol_vec;
            ContainerT        *state_vec;
            std::vector<uint8_t> *binary_vec;
            std::vector<cl_mem> mem_object_vec;

            char *sig_input;
            //node_data *node;
    };

    template<typename WorkTypes,
             typename ContainerT = std::vector<boost::unordered_map<char, size_t> >
             >
    class clutil_memory : public base_clutil
    {
        public:
            clutil_memory();

            void cl_create_buffer(WorkTypes& worktype_loads,
                   	std::vector<char>   & symbol_vec_ptr,
										std::vector<size_t> & state_vec_ptr,
                    std::vector<char>& binary_vec);

            bool cl_create_subbuffer();

            bool cl_check_buffer_size();

						bool cl_build_node_buffer_object() throw(std::runtime_error);

            bool cl_release_memory();

            ~clutil_memory();

            std::vector<boost::shared_ptr<platdevices_info> >&    get_platdevices_info_vec() {
                return  memory_util->get_platdevices_info_vec();
            }
            void set_platdevices_info_vec(std::vector<boost::shared_ptr<platdevices_info> >& ptr_info) {
                memory_util->set_platdevices_info_vec(ptr_info);
            }

            platdevices_info *get_platdevices_data() {
                return memory_util->get_platdevices_data();
            }

        private:
            memory<WorkTypes, ContainerT> *memory_util;

    };

}

#endif /*HNMAV_KERNEL_CLUTIL_HPP */
