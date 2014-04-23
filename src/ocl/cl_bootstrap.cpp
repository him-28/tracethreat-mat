#include "ocl/cl_bootstrap.hpp"

namespace hnmav_kernel
{

    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    set_opencl_file(std::string& opencl_file_path_ptr)
    {
        if(opencl_file_path_ptr.size() == 0) {
            return false;
        }

        opencl_file_path = &opencl_file_path_ptr;
        logger->write_info("Insert file path : ", *opencl_file_path);
        return true;
    }


    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    //cl_load_system(std::string& opencl_file_path)
    cl_load_system()
    //:opencl_file_path_(&opencl_file_path)
    {
        util::options_system& op_system = util::options_system::get_instance();
        //init logger
        logger_ptr = &util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();

        //logger->write_info("Load path opencl ", *opencl_file_path);
    }

    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::init_cl_system()
    {
        logger->write_info("---------------------- Start Load system ----------------------",
                format_type::type_center);

        if(!cl_load_platform()) {
            logger->write_info("!!!CANNOT LOAD PLATFORM");
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

        logger->write_info("---------------------- End Load system  ----------------------",
                format_type::type_center);
    }

    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool  cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    cl_load_platform()
    {

        utilplat  = new UtilPlatform;
        UtilPlatform   *util_platform = static_cast<UtilPlatform *>(utilplat);

        util_platform->get_info();

        util_platform->set_num_platforms_ids(0,NULL);
        util_platform->init_platforms_ids();

        if(util_platform->input_kernel_source(*opencl_file_path)) {

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

    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    cl_load_memory()
    {
        // Memory management  class
        base_memory_clutil = new clutil_memory<dstr::dstr_def::work_groupitems, std::vector<size_t> >();
        base_memory_clutil->set_platdevices_info_vec(utilplat->get_info());
        memory_clutil = static_cast<
                clutil_memory<dstr::dstr_def::work_groupitems, std::vector<size_t> > *
                >(base_memory_clutil);
        return true;
    }


    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    cl_build_memory()
    {
        //TODO: Check memory buffer before build

        bool ret = memory_clutil->cl_build_node_buffer_object();
        //memory_clutil->cl_check_buffer_size();

        if(!ret) {
            logger->write_info("cl_load_system::cl_init_memory, cannot build node buffer object");
            return false;
        }

        return true;
    }

    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    cl_process_buffer(std::vector<char>& symbol_vec_ptr,
            std::vector<int>&            state_vec_ptr,
            std::vector<char>&           binary_vec,
            std::vector<uint8_t>&       result_vec)
    {
        //WorkTypes workloads;
        //workloads.work_groups = 50;
        //workloads.work_items  = 100;
        dstr::dstr_def::work_groupitems workloads;

        logger->write_info_test("cl_load_system::cl_process_buffer, Symbol size",
                boost::lexical_cast<std::string>(symbol_vec_ptr.size()));
        logger->write_info_test("cl_load_system::cl_process_buffer, State size",
                boost::lexical_cast<std::string>(state_vec_ptr.size()));
        logger->write_info_test("cl_load_system::cl_process_buffer, File binary size",
                boost::lexical_cast<std::string>(binary_vec.size()));
				logger->write_info_test("cl_load_system::cl_process_buffer,  reuslt inex size",
								boost::lexical_cast<std::string>(result_vec.size()));

        memory_clutil->cl_create_buffer(workloads,
                symbol_vec_ptr, // symbol of signature.
                state_vec_ptr, // state of symbol.
                binary_vec,  // File binary char hex type.
                result_vec); // result array index.
    }


    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    cl_load_commandqueue()
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
    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    cl_process_commandqueue(std::vector<uint8_t> * result_vec)
    {
        comqueue_clutil->cl_create_command_queue();
        comqueue_clutil->cl_enqueue_nd_task(result_vec);
        return true;
    }


    /**
    * @brief Releases all cl processes.
    *
    * @return  True, If processes can be releases memory and all allocator.
    */
    template<typename UtilPlatform,
             typename TireDefine,
             typename WorkTypes,
             typename ContainerT
             >
    bool cl_load_system<UtilPlatform, TireDefine, WorkTypes, ContainerT>::
    cl_release_system()
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



    template class				 cl_load_system<clutil_platform,
                       dstr::dstr_def::work_groupitems,
                       std::vector<boost::unordered_map<char, size_t> >,
                       dstr::actire_parallel<char,
                       size_t,
                       boost::unordered_map<char, size_t>,
                       std::vector<boost::unordered_map<char, size_t> > >
                       >;



}


