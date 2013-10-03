#ifndef  HNMAV_KERNEL_CL_BOOTSTRAP_HPP
#define  HNMAV_KERNEL_CL_BOOTSTRAP_HPP

/*                       Titles                                          Authors                        Date
 *-Class load OpenCL                                                     Chatsiri.rat                   21/11/2012
 *-Add factory system style!!!                                           Chatsiri.rat                   26/11/2012
 *-Test load injection data node !!!
 */

//OpenCL utils
#include "data_structure/tire.hpp"
#include "utils/base_clutil.hpp"
#include "utils/clutil_platform.hpp"
#include "utils/clutil_memory.hpp"
#include "utils/clutil_commandqueue.hpp"
//path, log
#include "utils/config/options_system.hpp"
#include "utils/logger/clutil_logger.hpp"
#include "data_structure/absalgorithms.hpp"
#include "data_structure/structdef.hpp"
// initial
namespace hnmav_kernel
{

    namespace datastructure = hnmav_datastructure;
    namespace util = hnmav_util;

    namespace cl_bootstrap
    {
        template<typename UtilPlatform, 
					typename WorkTypes = datastructure::datastructure_def::work_groupitems, 
					typename TireDefine = datastructure::tire<std::string, char, std::list<std::vector<datastructure::node_tire<std::string, char> > >, datastructure::worktypes<int> > >
        class cl_load_system
        {
            public:
                cl_load_system(std::string& opencl_file_path);

                // initial load platform, memories and commandqueue
                bool init_cl_system();

                bool cl_load_platform();
                bool cl_load_memory();
                bool cl_load_commandqueue();

                bool cl_release_system();

                // read data from class scan_file.cpp
                bool cl_process_buffer(datastructure::absalgorithms<TireDefine, datastructure::datastructure_def::ntire_veclist>& buffer_node_vec);
                // Manage command queue
                bool cl_process_commandqueue();

            private:
                std::string *opencl_file_path_;

                UtilPlatform  *utilplat;
                clutil_memory<WorkTypes, TireDefine> *memory_clutil;
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

        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        cl_load_system<UtilPlatform, WorkTypes, TireDefine>::cl_load_system(std::string& opencl_file_path) : opencl_file_path_(&opencl_file_path)
        {
            util::options_system& op_system = util::options_system::get_instance();
            //init logger
            logger_ptr = &util::clutil_logging<std::string, int>::get_instance();
            logger = logger_ptr->get();

            logger->write_info("Load path opencl ", *opencl_file_path_);
        }

        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        bool cl_load_system<UtilPlatform, WorkTypes, TireDefine>::init_cl_system()
        {
            logger->write_info("---------------------- Start Load system ----------------------", format_type::type_center);

            if(!cl_load_platform()) {
                logger->write_info("!!!CANNOT LOAD PLATFORM", format_type::type_header);
                return true;
            }

            logger->write_info("LOAD PLATFORM COMPLETED");

            if(!cl_load_memory()) {
                logger->write_info("!!!CANNOT LOAD MEMORY");
            }

            logger->write_info("LOAD MEMORY COMPLETED");

            if(!cl_load_commandqueue()) {
                logger->write_info("!!!CANNOT LOAD COMMANDQUEUE");
            }

            logger->write_info("LOAD COMMANDQUEUE COMPLETED");

            logger->write_info("---------------------- End Load system  ----------------------", format_type::type_center);
        }

        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        bool  cl_load_system<UtilPlatform, WorkTypes, TireDefine>::cl_load_platform()
        {

            utilplat  = new UtilPlatform;
            UtilPlatform   *util_platform = static_cast<UtilPlatform *>(utilplat);

            util_platform->get_info();

            util_platform->set_num_platforms_ids(0,NULL);
            util_platform->init_platforms_ids();

            if(util_platform->input_kernel_source(*opencl_file_path_)) {

                util_platform->get_platforms_info(CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");
                util_platform->get_device_ids(CL_DEVICE_TYPE_ALL);

                util_platform->cl_get_devices_info();

                util_platform->context_properties();
                util_platform->cl_create_context();

                util_platform->cl_create_programs_with_source();
                util_platform->cl_build_programs();
            } else {
                logger->write_info("#### Cannot load kernel source ####", format_type::type_center);
            }

            return true;
        }

        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        bool cl_load_system<UtilPlatform, WorkTypes, TireDefine>::cl_load_memory()
        {
            // Memory management  class
            base_memory_clutil = new clutil_memory<TireDefine>();
            base_memory_clutil->set_platdevices_info_vec(utilplat->get_info());
            memory_clutil = static_cast<clutil_memory<WorkTypes, TireDefine> *>(base_memory_clutil);
            return true;
        }

        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        bool cl_load_system<UtilPlatform, WorkTypes, TireDefine>::cl_process_buffer(datastructure::absalgorithms<TireDefine, 
							datastructure::datastructure_def::ntire_veclist> & buffer_node_vec)
        {
            WorkTypes workloads;
            workloads.work_groups = 50;
            workloads.work_items  = 100;
            memory_clutil->cl_create_buffer(workloads, buffer_node_vec);
            memory_clutil->cl_check_buffer_size();
        }


        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        bool cl_load_system<UtilPlatform, WorkTypes, TireDefine>::cl_load_commandqueue()
        {
            base_comqueue_clutil = new clutil_commandqueue();
            base_comqueue_clutil->set_platdevices_info_vec(utilplat->get_info());
            // Load  command queue
            comqueue_clutil = static_cast<clutil_commandqueue *>(base_comqueue_clutil);
            comqueue_clutil->cl_create_kernel();

            return true;
        }

        /**
        * @brief  Command queue memory from host to device.
        *
        * @return True,If processing of queury memory completed.
        */
        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        bool cl_load_system<UtilPlatform, WorkTypes, TireDefine>::cl_process_commandqueue()
        {
            comqueue_clutil->cl_create_command_queue();

            comqueue_clutil->cl_enqueue_nd_task();

            // comqueue_clutil->cl_enqueue_task();

            comqueue_clutil->cl_read_buffer();
            //            comqueue_clutil->cl_write_event();

            //            comqueue_clutil->cl_call_kernel();

            return true;
        }

        /**
        * @brief Releases all cl processes.
        *
        * @return  True, If processes can be releases memory and all allocator.
        */
        template<typename UtilPlatform, typename WorkTypes, typename TireDefine>
        bool cl_load_system<UtilPlatform, WorkTypes, TireDefine>::cl_release_system()
        {
            //release buffer
            memory_clutil->cl_release_memory();
            //release kernel and command queue
            comqueue_clutil->cl_release_kernel();
            comqueue_clutil->cl_release_commandqueue();
            //release programs & context;
            utilplat->cl_release_program();
            utilplat->cl_release_context();
        }

    }

}

#endif /* HNMAV_KERNEL_CL_BOOTSTRAP_HPP */
