#ifndef  HNMAV_KERNEL_CL_BOOTSTRAP_HPP
#define  HNMAV_KERNEL_CL_BOOTSTRAP_HPP

/*                       Titles                                          Authors                        Date
 *-Class load OpenCL                                                     Chatsiri.rat                   21/11/2012
 *-Add factory system style!!!                                           Chatsiri.rat                   26/11/2012
 *-Test load injection data node !!!
 */

//OpenCL utils
//#include "data_structure/tire.hpp"
#include "ocl/utils/base_clutil.hpp"
#include "ocl/utils/clutil_platform.hpp"
#include "ocl/utils/clutil_memory.hpp"
#include "ocl/utils/clutil_commandqueue.hpp"
//path, log
#include "utils/config/options_system.hpp"
#include "utils/logger/clutil_logger.hpp"
//#include "data_structure/absalgorithms.hpp"
#include "data_structure/structdef.hpp"
// initial
namespace hnmav_kernel
{

    namespace dstr = data_structure;
    namespace util = hnmav_util;

    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT = std::vector<boost::unordered_map<char, size_t> >
             >
    class cl_load_system
    {
        public:
            cl_load_system();

            bool set_opencl_file(std::string& opencl_file_path);
            // initial load platform, memories and commandqueue
            bool init_cl_system();

            bool cl_load_platform();

            bool cl_load_memory();

						bool cl_build_memory();

            bool cl_load_commandqueue();

            bool cl_release_system();


            bool cl_process_buffer(std::vector<char>& symbol_vec_tr,
                    std::vector<int>&  state_vec_ptr,
                    std::vector<char>& binary_vec,
										std::vector<uint8_t> & result_vec);

            // Manage command queue
            bool cl_process_commandqueue();

        private:
            std::string *opencl_file_path;

            UtilPlatform  *utilplat;
            clutil_memory<dstr::dstr_def::work_groupitems, std::vector<size_t> > *memory_clutil;
            base_clutil   *base_memory_clutil;
            base_clutil   *base_comqueue_clutil;
            clutil_commandqueue *comqueue_clutil;

            typedef TireDefine  tire_vec;
            tire_vec *node_tire_vec;

            std::string signature_name;
            // Handeling logger
            shared_ptr<util::clutil_logging<std::string, int> > *logger_ptr;
            util::clutil_logging<std::string, int>              *logger;

            TireDefine *node_tire_input;
            TireDefine *node_tire_output;
    };

}

#endif /* HNMAV_KERNEL_CL_BOOTSTRAP_HPP */
