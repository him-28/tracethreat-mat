#include <numeric>
#include "ocl/utils/clutil_memory.hpp"


namespace kernel
{

    template<typename WorkTypes, typename ContainerT>
    memory<WorkTypes, ContainerT>::memory()
    {
        logger_ptr = &utils::clutil_logging<std::string, int>::get_instance();
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
            std::vector<char>&        symbol_vec_ptr,
            std::vector<int>&         state_vec_ptr,
            std::vector<char>&       	binary_vec_ptr,
            std::vector<uint8_t>&    	result_vec_ptr)
    {
        logger->write_info("### Start cl_create_buffer ###", utils::format_type::type_header);
        logger->write_info_test("PE File, size ", boost::lexical_cast<std::string>(binary_vec_ptr.size()));
        logger->write_info_test("Symbol size ", boost::lexical_cast<std::string>(symbol_vec_ptr.size()));
        logger->write_info_test("State  size ", boost::lexical_cast<std::string>(state_vec_ptr.size()));


        platdevices_info *platdevices = get_platdevices_data();

       
        //shared ptr get vector symbol and state.
        platdevices->node_symbol_vec = symbol_vec_ptr; 
        platdevices->node_state_vec  = state_vec_ptr;
        platdevices->node_binary_vec = binary_vec_ptr;

				
				//set result node. Value sets are zero.
        platdevices->node_result_vec = &result_vec_ptr;
				
				//Symbol and State search	
        symbol_vec = &platdevices->node_symbol_vec;
        state_vec  = &platdevices->node_state_vec;

        logger->write_info("### memory::cl_create_buffer, symbol_vec.size()",
                boost::lexical_cast<std::string>(symbol_vec->size()));

        logger->write_info("### memory::cl_create_buffer,  state_vec.size()",
                boost::lexical_cast<std::string>(state_vec->size()));

        logger->write_info("### memory::cl_create_buffer,  binary.size()",
                boost::lexical_cast<std::string>(platdevices->node_binary_vec.size()));

		   logger->write_info("### memory::cl_create_buffer,   result.size()",
                boost::lexical_cast<std::string>(platdevices->node_result_vec->size()));

        return true;
    }


    /**
    * @brief Buffer for node struct
    *
    * @param plat_info  Platform devices struct class
    * @param node  Node of struct
    */
    template<typename WorkTypes, typename ContainerT>
    bool memory<WorkTypes, ContainerT>::
    cl_build_node_buffer_object()throw(std::runtime_error)
    {
        platdevices_info *plat_info = get_platdevices_data();

        logger->write_info("### Start cl_build_node_buffer_object ###", utils::format_type::type_header);

        logger->write_info("   Symbol size    ",
                lexical_cast<std::string>(plat_info->node_symbol_vec.size()));

        logger->write_info("-- State  size    ",
                lexical_cast<std::string>(plat_info->node_state_vec.size()));

        logger->write_info("-- Binary Size    ",
                lexical_cast<std::string>(plat_info->node_binary_vec.size()));

        logger->write_info("-- Result Size    ",
                lexical_cast<std::string>(plat_info->node_result_vec->size()));

        cl_int err = CL_SUCCESS;
        int node_size = 0;

        try {
            // inital vector size
            plat_info->vec_buffer.initial_size(KERNEL_BUFFER_SIZE);
            //Node input
            cl_mem  symbol_mem = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(char) * plat_info->node_symbol_vec.size(),
                    &plat_info->node_symbol_vec[0],
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-node_symbol_vec");

            // Node->data initial root and child- 0
            plat_info->vec_buffer.push_back(symbol_mem);

            cl_mem  state_mem = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(int) * plat_info->node_state_vec.size(),
                    &plat_info->node_state_vec[0],
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-node_state_vec");

            // set size of buffer input - 1
            plat_info->vec_buffer.push_back(state_mem);

            // signature size
            cl_mem  binaryf_mem = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(char) * plat_info->node_binary_vec.size(), /* uint8_t : Old define type. */
                    &plat_info->node_binary_vec[0],
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-node_binary_vec");

            // set size of buffer input - 2
            plat_info->vec_buffer.push_back(binaryf_mem);

            //Write back symbol
            cl_mem  symbol_wb_mem = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(char) * plat_info->node_binary_vec.size(),
                    plat_info->symbol_wb,
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-symbol_wb");

            // set size of buffer input - 3
            plat_info->vec_buffer.push_back(symbol_wb_mem);

            //Write back index found.
            cl_mem  result_mem = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_WRITE_ONLY,
                    sizeof(uint8_t) * plat_info->node_result_vec->size(), //Equal node_result_vec.size()
                    &plat_info->node_result_vec[0],
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-node_result_vec");

            // set size of write back index found matching. - 4
            plat_info->vec_buffer.push_back(result_mem);

            //Write back symbol
            cl_mem  result_wb_mem = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_ONLY,
                    sizeof(int) * plat_info->node_binary_vec.size(),
                    plat_info->result_wb,
                    &err);

            if(err != CL_SUCCESS)
                throw except::cl::clutil_exception(err, "clCreateBuffer-Build-symbol_wb");

            // set size of buffer input - 5
            plat_info->vec_buffer.push_back(result_wb_mem);

        } catch(std::runtime_error&  ex) {
            logger->write_error( ex.what() );
            return false;
        }

        return true;
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
        logger->write_info("### Start cl_check buffer size ###",utils::format_type::type_header);
        platdevices_info *platdevices = get_platdevices_data();

        int count_mem_object = 0;
        int sum_signature = 0;

        char *signature_input;
        signature_input;// = new char[platdevices->node_tire_input->size()];
        logger->write_info("memory::::cl_check_buffer_size", boost::lexical_cast<std::string>(
                platdevices->vec_buffer.size()));

        if(platdevices->vec_buffer.size() == 0)
            return false;

        for(int count_vec = 0; count_vec <  platdevices->vec_buffer.size(); count_vec ++) {

            clGetMemObjectInfo(platdevices->vec_buffer[count_vec],
                    CL_MEM_SIZE,
                    sizeof(signature_input) * platdevices->num_devices,
                    signature_input,
                    NULL);
            sum_signature++;

        }

        logger->write_info("--- buffer size ",  lexical_cast<std::string>(sum_signature));
        return true;
    }

    template<typename WorkTypes, typename ContainerT>
    memory<WorkTypes, ContainerT>::~memory(){
			delete symbol_vec;
			delete state_vec;
			delete binary_vec;
			delete sig_input;
			delete platdevices;
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
            std::vector<char>&    symbol_vec_ptr,
            std::vector<int>& state_vec_ptr,
            std::vector<char>& binary_vec,
					  std::vector<uint8_t> & result_vec)
    {
        memory_util->cl_create_buffer(worktype_loads,
                symbol_vec_ptr,
                state_vec_ptr,
                binary_vec,
								result_vec);
    }

    template<typename WorkTypes, typename ContainerT>
    bool  clutil_memory<WorkTypes, ContainerT>::
    cl_build_node_buffer_object() throw(std::runtime_error)
    {

        return memory_util->cl_build_node_buffer_object();
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
			  memory_util->~memory();
    }

    template class memory<dstr::dstr_def::work_groupitems, std::vector<size_t> >;
    template class clutil_memory<dstr::dstr_def::work_groupitems, std::vector<size_t> >;


}


