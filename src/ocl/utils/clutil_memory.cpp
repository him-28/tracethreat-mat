#include "ocl/utils/clutil_memory.hpp"

namespace hnmav_kernel
{

    template<typename WorkTypes, typename ContainerT>
    memory<WorkTypes, ContainerT>::memory()
    {
        logger_ptr = &util::clutil_logging<std::string, int>::get_instance();
        logger     = logger_ptr->get();
    }

    /**
    * @brief Send abstract of tire to arrays_parallel
    *
    * @param worktype_loads
    * @param abs_tireall
    *
    * @return
    */
    template<typename WorkTypes, typename ContainerT>
    bool memory<WorkTypes, ContainerT>::cl_create_buffer(WorkTypes& worktype_loads,
            dstr::iparallel<char, size_t>& ipara,
            std::vector<uint8_t>&     binary_vec_ptr)
    {
        logger->write_info("### Start cl_create_buffer ###", util::format_type::type_header);
        platdevices_info *platdevices = get_platdevices_data();

        //get data from tuple by boost::tie
        /*boost::tie(platdevices->node_symbol_vec, platdevices->node_state_vec) =  ipara.get_container();

        symbol_vec = platdevices->node_symbol_vec;
        state_vec  = platdevices->node_state_vec;
        		binary_vec = binary_vec_ptr;
        		*/

        //shared ptr get vector symbol and state.
        platdevices->node_symbol_vec = *ipara.get_symbol_shared_ptr().get();
        platdevices->node_state_vec  = *ipara.get_state_shared_ptr().get();
        symbol_vec = &platdevices->node_symbol_vec;
        state_vec  = &platdevices->node_state_vec;

        logger->write_info("### memory::cl_create_buffer, symbol_vec.size()", 
									boost::lexical_cast<std::string>(symbol_vec->size()));
        logger->write_info("### memory::cl_create_buffer,  state_vec.size()", 
                  boost::lexical_cast<std::string>(state_vec->size()));

        return true;
    }


    /**
    * @brief Buffer for node struct
    *
    * @param plat_info  Platform devices struct class
    * @param node  Node of struct
    */
    template<typename WorkTypes, typename ContainerT>
    void memory<WorkTypes, ContainerT>::build_node_buffer_object(
            platdevices_info *plat_info)throw(std::runtime_error)
    {
        cl_int err = CL_SUCCESS;
        int node_size = 0;
        //logger->write_info("Node size insert ", lexical_cast<std::string>(sizeof(node)));
        //logger->write_info("-- Node all size    ", lexical_cast<std::string>(count_sig_input));
        //logger->write_info("-- size of Node data    ", lexical_cast<std::string>(sizeof(node_data)));
        /*
        char *data_str = (char *)malloc( count_sig_input * sizeof(char));

        for(int count_data = 0; count_data < count_sig_input; count_data++) {
        data_str[count_data] = node->data[count_data];
        }
        */

        try {
            // inital vector size
            plat_info->vec_buffer.initial_size(6);
            //Node input
            cl_mem  buffer_input = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(char) * plat_info->node_symbol_vec.size(),
                    &plat_info->node_symbol_vec[0],
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            // Node->data initial root and child
            plat_info->vec_buffer.push_back(buffer_input);

            cl_mem  sigrc_input = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(size_t) * plat_info->node_state_vec.size(),
                    &plat_info->node_state_vec[0],
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            // set size of buffer input
            plat_info->vec_buffer.push_back(sigrc_input);

            // signature size
            cl_mem  buffer_input_sizes = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(uint8_t) * plat_info->node_binary_vec.size(),
                    &plat_info->node_binary_vec[0],
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            // set size of buffer input
            plat_info->vec_buffer.push_back(buffer_input_sizes);

        } catch(std::runtime_error&  ex) {
            logger->write_error( ex.what() );
        }

    }



    /**
    * @brief Releases memory in buffer.
    *
    * @return True, Memory releases successe. False, Memory releases unsuccess and throws exceptions.
    */
    template<typename WorkTypes, typename ContainerT>
    bool memory<WorkTypes, ContainerT>::cl_release_memory()
    {
        //release memory object
        platdevices_info  *platdevices = get_platdevices_data();

        for(typename std::vector<cl_mem>::iterator iter_mem = platdevices->mem_input_buffers->begin();
                iter_mem != platdevices->mem_input_buffers->end();
                ++iter_mem) {
            clReleaseMemObject(*iter_mem);
        }

        logger->write_info("--- Release Memory Completed ");
    }

    template<typename WorkTypes, typename ContainerT>
    bool memory<WorkTypes, ContainerT>::cl_create_subbuffer()
    {
        cl_int err = CL_SUCCESS;
        platdevices_info  *platdevices = get_platdevices_data();
        return true;
    }


    /**
    * @brief Check Buffer size on GPU or GPU
    *
    * @return  False, Buffer cannot initial sizes of buffer.
    */
    template<typename WorkTypes, typename ContainerT>
    bool memory<WorkTypes, ContainerT>::cl_check_buffer_size()
    {
        logger->write_info("### Start cl_check buffer size ###",util::format_type::type_header);
        platdevices_info *platdevices = get_platdevices_data();

        int count_mem_object = 0;
        int sum_signature = 0;

        char *signature_input;
        signature_input;// = new char[platdevices->node_tire_input->size()];

        if(platdevices->mem_input_buffers->size() == 0)
            return false;

        for(std::vector<cl_mem>::iterator iter = platdevices->mem_input_buffers->begin();
                iter != platdevices->mem_input_buffers->end();
                ++iter) {
            clGetMemObjectInfo(*iter,
                    CL_MEM_SIZE,
                    sizeof(signature_input) * platdevices->num_devices,
                    signature_input,
                    NULL);
            sum_signature++;
        }

        logger->write_info("--- buffer size ",  lexical_cast<std::string>(sum_signature));
        return true;
    }

    /**
    * @brief Handling memory in GPU or CPU selected. Interface class for API.
    */


    template<typename WorkTypes, typename ContainerT>
    clutil_memory<WorkTypes, ContainerT>::clutil_memory()
    {
        memory_util = new memory<WorkTypes, ContainerT>();
    }

    template<typename WorkTypes, typename ContainerT>
    bool clutil_memory<WorkTypes, ContainerT>::cl_release_memory()
    {
        return memory_util->cl_release_memory();
    }

    template<typename WorkTypes, typename ContainerT>
    void  clutil_memory<WorkTypes, ContainerT>::cl_create_buffer(WorkTypes& worktype_loads,
            dstr::iparallel<char, size_t>& ipara,
            std::vector<uint8_t>& binary_vec)
    {
        memory_util->cl_create_buffer(worktype_loads, ipara, binary_vec);
    }

    template<typename WorkTypes, typename ContainerT>
    bool  clutil_memory<WorkTypes, ContainerT>::cl_create_subbuffer()
    {
        return	memory_util->cl_create_subbuffer();
    }

    template<typename WorkTypes, typename ContainerT>
    bool clutil_memory<WorkTypes,ContainerT>::cl_check_buffer_size()
    {
        return memory_util->cl_check_buffer_size();
    }

    template<typename WorkTypes, typename ContainerT>
    clutil_memory<WorkTypes, ContainerT>::~clutil_memory()
    {

    }

		template class memory<dstr::dstr_def::work_groupitems, std::vector<size_t> >;
    template class clutil_memory<dstr::dstr_def::work_groupitems, std::vector<size_t> >;


}


