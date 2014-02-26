#ifndef  HNMAV_KERNEL_CLUTIL_COMMANDQUEUE_HPP
#define  HNMAV_KERNEL_CLUTIL_COMMANDQUEUE_HPP

// name of buildtire kernel
//#define  BUILD_TIRE_KERNEL "buildtire"

#define  BUILD_TIRE_KERNEL "create_tire_buffer"

/*                       Titles                                          Authors           		Date
 *-Support command queue (Interface send/receive with vector<shared_ptr<platdevices_info> >
 *                                                                       Chatsiri.rat         07/09/2012
 *-Add input_str(Experimental with std::string). input_str is private variable
 * ( not plan include in struct name platdevices_info. )
 *                                                                       Chatsiri.rat         09/09/2012
 *-Add  memory buffer input & output member function.
 *                                                                       Chatsiri.rat         11/09/2012
 */


#include <boost/shared_ptr.hpp>

#include <iostream>

#include "ocl/cl_bootstrap.hpp"
#include "clutil_infodevice.hpp"


namespace kernel
{
    using namespace boost;
    using namespace hnmav_util;
    using namespace hnmav_exception;

    class commandqueue : public base_clutil
    {
        public:

            commandqueue() {
                logger_ptr = &clutil_logging<std::string, int>::get_instance();
                logger = logger_ptr->get();
            }

            bool cl_create_command_queue();
            //bool cl_write_event();
            //bool cl_read_buffer();

            bool cl_create_kernel();

            bool cl_release_kernel();
            bool cl_release_commandqueue();

            bool cl_enqueue_task();
            bool cl_enqueue_nd_task();

            bool cl_enqueue_copy_buffer();
            bool cl_enqueue_map_buffer();
            bool cl_enqueue_unmap_buffer();

            bool add_input_str(std::string input_str);

            void set_buffer_elements(std::size_t& buffer_elements_);


            // Interface class
            std::vector<shared_ptr<platdevices_info> >&    get_platdevices_info_vec() {
                return platdevices_shared_ptr;
            }
            void set_platdevices_info_vec(std::vector<shared_ptr<platdevices_info> >& ptr_info) {
                platdevices_shared_ptr = ptr_info;
            }

            platdevices_info *get_platdevices_data() {
                shared_ptr<platdevices_info>   plat_shared_ptr =  platdevices_shared_ptr.back();
                platdevices_info *platdevices =  plat_shared_ptr.get();
                return platdevices;
            }

        private:

            std::size_t *buffer_elements;
            std::vector<shared_ptr<platdevices_info> >  platdevices_shared_ptr;
            // cl define
            platdevices_info *platdevices;
            void   *mapped_memory;
            // logging
            shared_ptr<clutil_logging<std::string, int> > *logger_ptr;
            clutil_logging<std::string, int>   *logger;

            //node_data *node;
    };


    void commandqueue::set_buffer_elements(std::size_t& buffer_elements_)
    {
        this->buffer_elements = &buffer_elements_;
    }

    bool commandqueue::cl_create_kernel()
    {
        logger->write_info("### Start cl_create_kernel ###", format_type::type_header);

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
        logger->write_info("### Start cl_create_command_queue ###", format_type::type_header);

        cl_int err = CL_SUCCESS;
        platdevices_info *platdevices = get_platdevices_data();

        // get string input
        int  count_mem_object = 0;
        char signature_input[platdevices->node_tire_input->size()];

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
                logger->write_info("--- Address of mem_input_buffer  ",
                        lexical_cast<std::string>(platdevices->mem_input_buffers->size()));
                logger->write_info("--- Kernel size               ",
                        lexical_cast<std::string>(platdevices->kernels.size()));


                //node_input arguement
                err = clSetKernelArg(
                        platdevices->kernels[0],
                        0,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(0));

                //node root and child  arguement
                err = clSetKernelArg(
                        platdevices->kernels[0],
                        1,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(1));


                //size buffer
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        2,
                        sizeof(int),
                        platdevices->vec_buffer.pop_index(2));

                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        3,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(3));

                //global id size
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        4,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(4));

                //global id size
                err |= clSetKernelArg(
                        platdevices->kernels[0],
                        5,
                        sizeof(cl_mem),
                        platdevices->vec_buffer.pop_index(5));



                if(err != CL_SUCCESS)
                    throw cl::clutil_exception(err, "clCreateKernelArg");

            }

        } catch(std::runtime_error  ex) {
            logger->write_info( ex.what() );
            return false;
        }

        return true;
    }

    bool commandqueue::cl_enqueue_nd_task()
    {
        logger->write_info("#### Start cl_enqueue_nd_task ####", format_type::type_header);

        cl_int err = CL_SUCCESS;
        platdevices_info *platdevices = get_platdevices_data();

        try {

						logger->write_info("--- Global Size NDRange ",
										lexical_cast<std::string>(platdevices->global_size));

						logger->write_info("--- Local Size NDRange ",
										lexical_cast<std::string>(platdevices->local_size));

            std::size_t offset = 0;

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
            }

            //clWaitForEvents(platdevices->events.size(), const_cast<cl_event *>(&platdevices->events[0]));
        } catch(std::runtime_error  ex) {
            logger->write_error( ex.what() );
            return false;
        }

        return true;
    }



    bool commandqueue::cl_enqueue_task()
    {
        logger->write_info("#### Start cl_enqueue_task ... ####", format_type::type_header);

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
        logger->write_info("#### Start Unmap buffer ####", format_type::type_header);

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

    bool commandqueue::cl_enqueue_map_buffer()
    {
        logger->write_info("#### Start enqueue map buffer ####", format_type::type_header);

        platdevices_info *platdevices =  get_platdevices_data();

        try {
            cl_int err = CL_SUCCESS;

            logger->write_info("--- Mem copy check size   ", lexical_cast<std::string>((sizeof(cl_char) * platdevices->buffer_elements  * platdevices->num_devices)));
            logger->write_info("--- Queue size            ", lexical_cast<std::string>(platdevices->queues.back()));

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

		/*
    bool commandqueue::cl_write_event()
    {
        logger->write_info( "#### Start cl_write_event ####", format_type::type_header );

        try {


            platdevices_info *platdevices = get_platdevices_data();

            int  count_input = 0;
//            char *signature_input = new char[platdevices->node_tire_input->size()]();
						char *signature_input = (char*)malloc(platdevices->node_tire_input->size() * sizeof(char));
            for(typename std::vector<node_data>::iterator iter_data = platdevices->node_tire_input->begin();
                    iter_data != platdevices->node_tire_input->end();
                    ++iter_data, ++count_input) {

                node_data  input_data = *iter_data;
                signature_input[count_input] = input_data.data[count_input];
            }

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
		*/
		
    /**
    * @brief Read buffer from devices to host.
    *
    * @return True, if can read buffer of devices. False, Throws error of  clEnqueueReadBuffer.
    */
    /*bool commandqueue::cl_read_buffer()
    {
        logger->write_info("#### Start cl_read_buffer ####", format_type::type_header);

        try {

            cl_int err = CL_SUCCESS;

            platdevices_info *platdevices = get_platdevices_data();

            node = new node_data[platdevices->mem_input_buffers_sizes];
            char output[platdevices->mem_input_buffers_sizes];

            logger->write_info(">Output declare sizes of buffer ", lexical_cast<std::string>(platdevices->mem_input_buffers_sizes));

            err = clEnqueueReadBuffer(
                    platdevices->queues[0],
                    *platdevices->vec_buffer.pop_index(0),
                    CL_TRUE,
                    0,
                    sizeof(node_data) * platdevices->mem_input_buffers_sizes,
                    node,
                    0,
                    NULL,
                    NULL);

            for(int count_output = 0; count_output  < platdevices->mem_input_buffers_sizes; count_output++)
                output[count_output] = node->data[count_output];

              
            std::vector<char> node_vec(output, output+platdevices->mem_input_buffers_sizes);
            std::string output_str(node_vec.begin(), node_vec.end());

            logger->write_info(">Signature output ", output_str);

            char   *sigrc = (char *)malloc(platdevices->mem_input_buffers_sizes * sizeof(char));

            for(int countsig = 0; countsig < platdevices->mem_input_buffers_sizes; countsig++) {
                sigrc[countsig] = '\n';
            }

            err |= clEnqueueReadBuffer(
                    platdevices->queues[0],
                    *platdevices->vec_buffer.pop_index(5),
                    CL_TRUE,
                    0,
                    sizeof(char) * platdevices->mem_input_buffers_sizes,
                    sigrc,
                    0,
                    NULL,
                    NULL);

            for(int i = 0; i < 5; i++)
                logger->write_info(">Loop output sigrc ", lexical_cast<std::string>(sigrc[i]));

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clEnqueueWriteBuffer");

        } catch(std::runtime_error ex) {
            logger->write_error(ex.what());
            return false;
        }

        return true;
    }*/


    bool commandqueue::add_input_str(std::string input_str)
    {
        platdevices_info *platdevices = get_platdevices_data();

        if(input_str.size() == 0)
            return false;

        /*platdevices->input_str = input_str;*/
        return true;
    }

    class clutil_commandqueue : public base_clutil
    {
        public:
            clutil_commandqueue() : commandqueue_util(new commandqueue()) { }

            bool cl_create_command_queue();
            //bool cl_write_event();
           // bool cl_read_buffer();
            bool cl_create_kernel();

            bool cl_enqueue_task();
            bool cl_enqueue_nd_task();

            bool cl_enqueue_copy_buffer();
            bool cl_enqueue_map_buffer();

            bool cl_enqueue_unmap_buffer();

            bool cl_release_kernel();
            bool cl_release_commandqueue();

            bool add_input_str(std::string input_str);
            void set_buffer_elements(std::size_t& buffer_elements_);

            // Interface class
            std::vector<shared_ptr<platdevices_info> >&    get_platdevices_info_vec() {
                return  commandqueue_util->get_platdevices_info_vec();
            }
            void set_platdevices_info_vec(std::vector<shared_ptr<platdevices_info> >& ptr_info) {
                commandqueue_util->set_platdevices_info_vec(ptr_info);
            }

            platdevices_info *get_platdevices_data() {
                return commandqueue_util->get_platdevices_data();
            }

        private:
            commandqueue *commandqueue_util;
    };


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
		/*
    bool clutil_commandqueue::cl_write_event()
    {
        return commandqueue_util->cl_write_event();
    }
		*/
    bool clutil_commandqueue::cl_enqueue_nd_task()
    {
        return commandqueue_util->cl_enqueue_nd_task();
    }
		/*
    bool clutil_commandqueue::cl_read_buffer()
    {
        return commandqueue_util->cl_read_buffer();
    }
		*/

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

    bool clutil_commandqueue::cl_enqueue_unmap_buffer()
    {
        return commandqueue_util->cl_enqueue_unmap_buffer();
    }

    void clutil_commandqueue::set_buffer_elements(std::size_t& buffer_elements_)
    {
        commandqueue_util->set_buffer_elements(buffer_elements_);
    }


}



#endif /* HNMAV_KERNEL_CLUTIL_COMMANDQUEUE_HPP */
