
#include "CL/cl.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "utils/base/system_code.hpp"

#include "ocl/utils/clutil_commandqueue.hpp"
//GPU OCL new namespace

namespace hnmav_kernel
{

    void commandqueue::set_buffer_elements(std::size_t& buffer_elements_)
    {
        this->buffer_elements = &buffer_elements_;
    }

    bool commandqueue::cl_create_kernel()
    {
        logger->write_info("Start cl_create_kernel", format_type::type_header);

        try {
            cl_int err = CL_SUCCESS;
            platdevices_info *platdevices = get_platdevices_data();

            logger->write_info("--- Kernel programs ", lexical_cast<std::string>(platdevices->program));

            cl_kernel kernel_tire = clCreateKernel(
                    platdevices->program,
                    BUILD_TIRE_KERNEL,
                    &err);

            platdevices->kernels.push_back(kernel_tire);

            logger->write_info("--- Kernel tire ", lexical_cast<std::string>(platdevices->kernels[0]));


            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateKernel");

            logger->write_info("--- Create kernel success");

            return true;

        } catch(std::runtime_error  ex) {
            logger->write_info( ex.what() );
            return false;
        }
    }

    /**
    * @brief Create Command queues. Create Kernel Arguments of buffering on memory  at member function.
    *
    * @return False, It's cannot set kernel arguments for buffer.
    */
    bool commandqueue::cl_create_command_queue()
    {
        logger->write_info("Start cl_create_command_queue", format_type::type_header);

        cl_int err = CL_SUCCESS;
        platdevices_info *platdevices = get_platdevices_data();

        //calculate global,local size of thread.
        cl_uint total_search_pos = platdevices->node_binary_vec.size() -
                platdevices->node_symbol_vec.size() +1;

        int search_length_wg =  SEARCH_BYTES_PER_ITEM * LOCAL_SIZE;
        int workgroup_count  =  (total_search_pos + search_length_wg - 1)/search_length_wg;


        platdevices->local_size  = LOCAL_SIZE;
        //24;//platdevices->node_symbol_vec.size();  //symbol_vec : 32
        platdevices->global_size = platdevices->local_size * workgroup_count;
        //1024;//platdevices->node_binary_vec.size();  //symbol_vec : 1024
        //Binary length kernel argv size.
        platdevices->binary_length = platdevices->node_binary_vec.size();
        //Symbol length kernel argv size
        platdevices->symbol_length = platdevices->node_symbol_vec.size();
        //search_length_wg
        platdevices->search_length_wg = search_length_wg;

        // get string input
        int  count_mem_object = 0;
        char signature_input;

        try {

            for(unsigned int count_queue  = 0; count_queue < platdevices->num_devices; count_queue++) {

                InfoDevice<cl_device_type>::display(
                        platdevices->device_ids[count_queue],
                        CL_DEVICE_TYPE,
                        "CL_DEVICE_TYPE");


                cl_command_queue command_queue = clCreateCommandQueue(
                        platdevices->context,
                        platdevices->device_ids[count_queue],
                        0,
                        &err);

                if(err != CL_SUCCESS)
                    throw cl::clutil_exception(err, "clCreateCommandQueue");

                platdevices->queues.push_back(command_queue);

                logger->write_info("--- Address of command queue  ",
                        lexical_cast<std::string>(command_queue));
                logger->write_info("--- Count_queue               ",
                        lexical_cast<std::string>(count_queue));
                logger->write_info("--- Kernel size               ",
                        lexical_cast<std::string>(platdevices->kernels.size()));


                //node symbol
                err = clSetKernelArg(
                        platdevices->kernels[0],
                        0,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(0));

                //node state
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        1,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(1));


                //size byte char compare
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        2,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(2));

                //size byte char compare
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        3,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(3));

                //size byte result compare
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        4,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(4));

                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        5,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(5));

                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue, Cannot create kernel arg 5");
                    throw cl::clutil_exception(err, "clCreateKernelArg");
                }

                //binary length
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        6,
                        sizeof(cl_uint),
                        (void *)&platdevices->binary_length);

                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue, Cannot create kernel arg 6");
                    throw cl::clutil_exception(err, "clCreateKernelArg");
                }

                //symbol(pattern) length
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        7,
                        sizeof(cl_uint),
                        (void *)&platdevices->symbol_length);


                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue, Cannot create kernel arg 7");
                    throw cl::clutil_exception(err, "clCreateKernelArg");
                }


                //max length
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        8,
                        sizeof(cl_uint),
                        (void *)&platdevices->search_length_wg);

                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue, Cannot create kernel arg 8");
                    throw cl::clutil_exception(err, "clCreateKernelArg");
                }


                //symbol(pattern) length; local
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        9,
                        platdevices->node_symbol_vec.size(),
                        NULL);



                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue, Cannot create kernel arg 9");
                    throw cl::clutil_exception(err, "clCreateKernelArg");
                }

                // Fix get queue
                //Write symbol to queue
                cl_mem  cl_mem_symbol = platdevices->vec_buffer[0];
                err |= clEnqueueWriteBuffer(platdevices->queues[0],
                        cl_mem_symbol,
                        CL_TRUE,
                        0,
                        sizeof(char) * platdevices->node_symbol_vec.size(),
                        (void *)&platdevices->node_symbol_vec[0],
                        0,
                        NULL,
                        NULL);

                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue, \
     									 Write clEnqueueWriteBuffer 01 error");
                    throw cl::clutil_exception(err, "clEnqueueWriteBuffer");
                }

                //Write state to queue
                cl_mem cl_mem_state = platdevices->vec_buffer[1];

                err |= clEnqueueWriteBuffer(platdevices->queues[0],
                        cl_mem_state,
                        CL_TRUE,
                        0,
                        sizeof(int) * platdevices->node_state_vec.size(),
                        (void *)&platdevices->node_state_vec[0],
                        0,
                        NULL,
                        NULL);

                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue, \
      								Write clEnqueueWriteBuffer 02 error");
                    throw cl::clutil_exception(err, "clEnqueueWriteBuffer");
                }

                //Binary of file to queue
                cl_mem cl_mem_binary = platdevices->vec_buffer[2];

                logger->write_info_test("commandqueue::cl_create_command_queue, node_binary_vec size ",
                        boost::lexical_cast<std::string>(platdevices->node_binary_vec.size()));

                err |= clEnqueueWriteBuffer(platdevices->queues[0],
                        cl_mem_binary,
                        CL_TRUE,
                        0,
                        sizeof(char) * platdevices->node_binary_vec.size(),
                        (void *)&platdevices->node_binary_vec[0],
                        0,
                        NULL,
                        NULL);

                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue,\
											 clEnqueueWriteBuffer 03 error");
                    throw cl::clutil_exception(err, "clEnqueueWriteBuffer");
                }


                cl_mem cl_mem_result = platdevices->vec_buffer[4];
								/*
                logger->write_info_test("commandqueue::cl_create_command_queue, node_result_vec size ",
                        boost::lexical_cast<std::string>(platdevices->node_result_vec->size()));
                logger->write_info_test("commandqueue::cl_create_command_queue, result [0] ",
                        boost::lexical_cast<std::string>(platdevices->node_result_vec->at(0)));
								*/
                err |= clEnqueueWriteBuffer(platdevices->queues[0],
                        cl_mem_result,
                        CL_FALSE,
                        0,
                        sizeof(uint8_t) * platdevices->node_result_vec->size(),
                        (void *)&platdevices->node_result_vec->at(0),
                        0,
                        NULL,
                        NULL);

                if(err != CL_SUCCESS) {
                    logger->write_info("commandqueue::cl_create_command_queue,\
											 clEnqueueWriteBuffer 04 error");
                    throw cl::clutil_exception(err, "clEnqueueWriteBuffer");
                }


            }

        } catch(std::runtime_error  ex) {
            logger->write_info( ex.what() );
            return false;
        }

        return true;
    }

    /*Magic code for test : commandqueue::cl_enqueue_nd_task  */
    //int size_symbol_bw = platdevices->node_symbol_vec.size();

    //const char *binary_test = "e1fba0e00b409cd21b8014ccd215468";
    //const char *binary_end  = binary_test + strlen(binary_test);
    //std::vector<char>  data_check;
    //data_check.insert(data_check.end(), binary_test, binary_end);

    /*
     for(int count_bin = 129; count_bin < 180; count_bin++) {
     if(platdevices->node_binary_vec[count_bin] == data_check[0]) {
     printf("Data equal: %c, index : %d \n",
             platdevices->node_binary_vec[count_bin],
             count_bin);

     for(int count_data = 1; count_data < data_check.size(); count_data++) {
         count_bin++;

         if(platdevices->node_binary_vec[count_bin] == data_check[count_data]) {
             printf("Data equal: %c, index : %d \n",
                     platdevices->node_binary_vec[count_bin],
                     count_bin);

         } else {
             //printf("-----break-----\n");
             break;
         }
     }
     }
     }
     */
    /*
    for(int count_symbol = 129;
    count_symbol < 180;//platdevices->node_binary_vec.size();
    count_symbol++) {

    //if( platdevices->result_wb[count_symbol] > 0){
    printf("print value int : %d, index : %d \n",
    platdevices->result_wb[count_symbol],
    count_symbol);
    //}

    }//for
    */

    /*
    printf("\n----- Hex test -------\n");
    int count_in = 0;

    //defaul 140-160, 1215370 - 1215390
    for(int countb = 65530; countb < 65550; countb++) {
        printf("| data : %c , index : %d | ", platdevices->node_binary_vec[countb], countb);
    }

    for(int countb = 65530; countb < 65550; countb++) {
        printf("%c", platdevices->node_binary_vec[countb]);
    }


    printf("\n----- End Hex --------\n");

    printf("\n------ State----------\n");

    for(int counts = 0; counts < platdevices->node_symbol_vec.size(); counts++) {

        printf("AC Parallel, Data :%c , State : %d \n",
                platdevices->node_symbol_vec[counts], platdevices->node_state_vec[counts]);

    }

    printf("\n------ End Symbol---------\n");
    		*/

    bool commandqueue::cl_enqueue_nd_task(std::vector<uint8_t> *result_vec)
    {
        logger->write_info("Start cl_enqueue_nd_task", format_type::type_header);

        cl_int err = CL_SUCCESS;
        platdevices_info *platdevices = get_platdevices_data();

        try {

            //Calculate work size.
            std::size_t offset = 0;

            logger->write_info("--- Global Size NDRange ",
                    lexical_cast<std::string>(platdevices->global_size));

            logger->write_info("--- Local Size NDRange ",
                    lexical_cast<std::string>(platdevices->local_size));

            for(int count_queue = 0; count_queue < platdevices->queues.size(); count_queue++) {
                cl_event event;
                err = clEnqueueNDRangeKernel(
                        platdevices->queues[count_queue],
                        platdevices->kernels[count_queue],
                        1,
                        &offset,
                        &platdevices->global_size,
                        &platdevices->local_size,
                        0,
                        NULL,
                        &event);

                if(err != CL_SUCCESS)
                    throw cl::clutil_exception(err, "clEnqueueNDRageKernel");

                platdevices->events.push_back(event);
                logger->write_info("--- NDRange-Kernel ",
                        lexical_cast<std::string>(count_queue));

                //support 1 queue : 00003
                break;
            }//for

            logger->write_info_test("commandqueue::cl_create_command_queue, test read back");

            platdevices->result_wb  = (int *)malloc(sizeof(int)  * platdevices->node_binary_vec.size());
            platdevices->result_group_wb = (int *)malloc(sizeof(int) * platdevices->node_binary_vec.size());

            //Fill node binary result with
            std::fill(platdevices->result_wb,
                    platdevices->result_wb + platdevices->node_binary_vec.size(),
                    1);

            //Node result vec initial and fill value.
            //platdevices->node_result_vec = new std::vector<uint8_t>(platdevices->node_binary_vec.size());
            //std::fill(platdevices->node_result_vec.begin(),
            //        platdevices->node_result_vec.end(),
            //        0);

            //write back- test only
            err |= clEnqueueReadBuffer(platdevices->queues[0],
                    *platdevices->vec_buffer.pop_index(4), //3
                    CL_TRUE,
                    0,
                    sizeof(char) * platdevices->node_binary_vec.size(),
                    platdevices->result_group_wb, //symbol_wb
                    0,
                    NULL,
                    NULL);

            if(err != CL_SUCCESS) {
                logger->write_info("commandqueue::cl_create_command_queue, 03,\
											 clEnqueueReadBuffer  error");
                throw cl::clutil_exception(err, "clEnqueueReadBuffer");
            }

            //write back- test only
            err |= clEnqueueReadBuffer(platdevices->queues[0],
                    *platdevices->vec_buffer.pop_index(5),
                    CL_TRUE,
                    0,
                    sizeof(int) * platdevices->node_binary_vec.size(),
                    platdevices->result_wb,
                    0,
                    NULL,
                    NULL);

            if(err != CL_SUCCESS) {
                logger->write_info("commandqueue::cl_create_command_queue, 05,\
											 clEnqueueReadBuffer  error");
                throw cl::clutil_exception(err, "clEnqueueReadBuffer");
            }

            uint64_t index_result = 0;

            while(index_result <= platdevices->node_binary_vec.size()) {
                if(platdevices->result_wb[index_result] > 0) {
                    int *hex_bin = &platdevices->result_wb[index_result]; //&, symbol_wb
                    int *hex_bin_group = &platdevices->result_group_wb[index_result];

                    if(*hex_bin > platdevices->node_binary_vec.size())
                        break;


                    if(platdevices->node_binary_vec[*hex_bin]) {

                        //Data :  platdevices->node_binary_vec[*hex_bin]
                        //Group index : *hex_bin_group
                        //Infected found bits set to node_binary_vec

                        logger->write_info_test(" commandqueue::cl_enqueue_nd_task, binary found index",
                                boost::lexical_cast<std::string>(*hex_bin));

                        uint8_t * write_index = &platdevices->node_result_vec->at(*hex_bin);

                        *write_index = utils::infected_found;

                    }
                }

                index_result++;

            }


            //Release memory
            for(int count_mem = 0; count_mem < platdevices->vec_buffer.size(); count_mem++) {
                clReleaseMemObject(platdevices->vec_buffer[count_mem]);
            }

            cl_release_kernel();
            cl_release_commandqueue();
            clReleaseProgram(platdevices->program);
            clReleaseContext(platdevices->context);

        } catch(std::runtime_error  ex) {
            logger->write_error( ex.what() );
            return false;
        }

        return true;
    }




    bool commandqueue::cl_enqueue_task()
    {
        logger->write_info("Start cl_enqueue_task ...", format_type::type_header);

        cl_int err = CL_SUCCESS;
        platdevices_info *platdevices = get_platdevices_data();

        try {
            err = clEnqueueTask(
                    platdevices->queues.back(),
                    platdevices->kernels.back(),
                    0,
                    NULL,
                    NULL);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err,"clEnqueueTask");

        } catch(std::runtime_error ex) {
            logger->write_error(ex.what());
            return false;
        }

        return true;
    }

    bool commandqueue::cl_release_kernel()
    {
        platdevices_info *platdevices = get_platdevices_data();

        for(typename std::vector<cl_kernel>::iterator iter_kernel = platdevices->kernels.begin();
                iter_kernel != platdevices->kernels.end();
                ++iter_kernel) {
            clReleaseKernel(*iter_kernel);
            logger->write_info("--- Release Kernel Completed");
        }

        return true;
    }

    bool commandqueue::cl_release_commandqueue()
    {
        platdevices_info *platdevices = get_platdevices_data();

        for(std::vector<cl_command_queue>::iterator iter_comqueue = platdevices->queues.begin();
                iter_comqueue != platdevices->queues.end();
                ++iter_comqueue) {
            clReleaseCommandQueue(*iter_comqueue);
        }

        logger->write_info("--- Release Command Queue Completed");
        return true;
    }



    bool commandqueue::cl_enqueue_unmap_buffer()
    {
        logger->write_info("Start Unmap buffer", format_type::type_header);

        platdevices_info *platdevices = get_platdevices_data();
        cl_int err = CL_SUCCESS;

        try {
            err = clEnqueueUnmapMemObject(
                    platdevices->queues.back(),
                    platdevices->mem_copy_buffers->back(),
                    mapped_memory,
                    0,
                    NULL,
                    NULL);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clEnqueueUnMemObject");

            logger->write_info("### Unmap memory complete ###");
        } catch(std::runtime_error ex) {
            std::cout<< ex.what() <<std::endl;
        }

    }

    bool commandqueue::cl_enqueue_map_buffer(cl_mem device_buffer,
            void  *&host_ptr,
            size_t size_bytes,
            cl_map_flags flags)
    {
        platdevices_info *platdevices =  get_platdevices_data();
        cl_int state;
        host_ptr = (void *)clEnqueueMapBuffer(platdevices->queues.back(),
                device_buffer,
                CL_TRUE,
                flags,
                0,
                size_bytes,
                0,
                NULL,
                NULL,
                &state);

        if(state!= CL_SUCCESS) {
            return false;

        }

        logger->write_info("commandqueue::cl_enqueue_map_buffer, code return :",
                boost::lexical_cast<std::string>(state));
        return true;
    }

    bool commandqueue::cl_enqueue_map_buffer()
    {
        logger->write_info("Start enqueue map buffer", format_type::type_header);

        platdevices_info *platdevices =  get_platdevices_data();

        try {
            cl_int err = CL_SUCCESS;

            logger->write_info("--- Mem copy check size   ",
                    lexical_cast<std::string>((sizeof(cl_char) *
                            platdevices->buffer_elements  *
                            platdevices->num_devices)));
            logger->write_info("--- Queue size            ",
                    lexical_cast<std::string>(platdevices->queues.back()));

            mapped_memory  =  clEnqueueMapBuffer(
                    platdevices->queues.back(),
                    platdevices->mem_copy_buffers->back(),
                    CL_TRUE,
                    CL_MAP_READ,
                    0,
                    sizeof(cl_char) * platdevices->buffer_elements  * platdevices->num_devices,
                    0,
                    NULL,
                    NULL,
                    &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clEnqueueMapBuffer");

            logger->write_info("### Map memory complete ###");

            char  mem_result[platdevices->buffer_elements];
            memcpy(mem_result, mapped_memory,  platdevices->buffer_elements);

        } catch(std::runtime_error ex) {
            logger->write_error(ex.what());
        }

    }

    bool commandqueue::cl_enqueue_copy_buffer()
    {
        logger->write_info("#### Start enqueue copy buffer ####", format_type::type_header);

        cl_int err = CL_SUCCESS;
        platdevices_info *platdevices = get_platdevices_data();

        try {
            err = clEnqueueCopyBuffer(
                    platdevices->queues.back(),
                    platdevices->mem_input_buffers->back(),
                    platdevices->mem_copy_buffers->back(),
                    0,
                    0,
                    sizeof(cl_char) * platdevices->buffer_elements * platdevices->num_devices,
                    0,
                    NULL,
                    NULL);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clEnqueueCopyBuffer");

        } catch(std::runtime_error  ex) {
            logger->write_error(ex.what());
            return false;
        }

        return true;
    }


    bool commandqueue::cl_write_event()
    {
        logger->write_info( "#### Start cl_write_event ####", format_type::type_header );

        try {


            platdevices_info *platdevices = get_platdevices_data();

            int  count_input = 0;
            char *signature_input;


            cl_mem buffer          = platdevices->mem_input_buffers->at(0);
            cl_command_queue queue = platdevices->queues.at(0);

            cl_int err = CL_SUCCESS;
            err =  clEnqueueWriteBuffer(
                    queue,
                    buffer,
                    CL_TRUE,
                    0,
                    sizeof(char) * strlen(signature_input),
                    &signature_input,
                    0,
                    NULL,
                    NULL);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clEnqueueWriteBuffer");
        } catch(std::runtime_error ex) {
            logger->write_error(ex.what());
            return false;
        }

        return true;
    }


    bool commandqueue::add_input_str(std::string input_str)
    {
        platdevices_info *platdevices = get_platdevices_data();

        if(input_str.size() == 0)
            return false;

        return true;
    }

    commandqueue::~commandqueue()
    {
        delete buffer_elements;
        delete platdevices;
    }


    bool clutil_commandqueue::cl_release_kernel()
    {
        return commandqueue_util->cl_release_kernel();
    }

    bool clutil_commandqueue::cl_release_commandqueue()
    {
        return commandqueue_util->cl_release_commandqueue();
    }

    bool clutil_commandqueue::cl_create_kernel()
    {
        return commandqueue_util->cl_create_kernel();
    }

    bool clutil_commandqueue::cl_create_command_queue()
    {
        return commandqueue_util->cl_create_command_queue();
    }

    bool clutil_commandqueue::cl_write_event()
    {
        return commandqueue_util->cl_write_event();
    }

    bool clutil_commandqueue::cl_enqueue_nd_task(std::vector<uint8_t> *result_vec)
    {
        return commandqueue_util->cl_enqueue_nd_task(result_vec);
    }


    bool clutil_commandqueue::add_input_str(std::string input_str)
    {
        return commandqueue_util->add_input_str(input_str);
    }

    bool clutil_commandqueue::cl_enqueue_task()
    {
        return commandqueue_util->cl_enqueue_task();
    }

    bool clutil_commandqueue::cl_enqueue_copy_buffer()
    {
        return  commandqueue_util->cl_enqueue_copy_buffer();
    }

    bool clutil_commandqueue::cl_enqueue_map_buffer()
    {
        return commandqueue_util->cl_enqueue_map_buffer();
    }


    bool clutil_commandqueue::cl_enqueue_map_buffer(cl_mem device_buffer,
            void  *&host_ptr,
            size_t size_bytes,
            cl_map_flags flags)
    {
        return commandqueue_util->cl_enqueue_map_buffer(device_buffer,
                host_ptr,
                size_bytes,
                flags);
    }


    bool clutil_commandqueue::cl_enqueue_unmap_buffer()
    {
        return commandqueue_util->cl_enqueue_unmap_buffer();
    }

    void clutil_commandqueue::set_buffer_elements(std::size_t& buffer_elements_)
    {
        commandqueue_util->set_buffer_elements(buffer_elements_);
    }

    clutil_commandqueue::~clutil_commandqueue()
    {
        commandqueue_util->~commandqueue();
    }

}
