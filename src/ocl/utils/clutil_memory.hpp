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
// Tire initial class for insert to OpenCL
#include "data_structure/tire.hpp"
// Exceptions & Logging
#include "utils/logger/clutil_logger.hpp"
//- Data structure
#include "data_structure/vector.hpp"
// Array parallel
#include "data_structure/arrays_parallel.hpp"
// Define type
#include "data_structure/structdef.hpp"
// abstract algorithms
#include "data_structure/absalgorithms.hpp"

namespace hnmav_kernel
{

    using namespace boost;

    namespace datastructure   = hnmav_datastructure;
    namespace util         = hnmav_util;
    namespace except       = hnmav_exception;


    //  memory internal initial, write Real Object here.
    template<typename WorkTypes,
             typename TireDefine = datastructure::tire<std::string,
             char,
             std::list<std::vector<datastructure::node_tire<std::string, char> > >,
             datastructure::worktypes<int> > >
    class memory : public base_clutil
    {
        public:

            memory();

            bool  cl_create_buffer(WorkTypes& worktype_loads,
                    datastructure::absalgorithms<TireDefine,
                    datastructure::datastructure_def::ntire_veclist>& buffer_vec_str);

            void  build_node_buffer_object(platdevices_info *plat_info, node_data *node, int count_sig_input) throw(std::runtime_error);

            bool  cl_create_subbuffer();
            bool  cl_check_buffer_size();

            bool  cl_release_memory();

            // Interface class
            std::vector<shared_ptr<platdevices_info> >&    get_platdevices_info_vec() {
                return platdevices_shared_ptr;
            }
            void set_platdevices_info_vec(std::vector<shared_ptr<platdevices_info> >& ptr_info) {
                platdevices_shared_ptr = ptr_info;
            }

            platdevices_info *get_platdevices_data() {
                shared_ptr<platdevices_info>   plat_shared_ptr = platdevices_shared_ptr.back();
                platdevices_info *platdevices =  plat_shared_ptr.get();
                return platdevices;
            }


        private:

            std::size_t buffer_elements;
            std::vector<shared_ptr<platdevices_info> >  platdevices_shared_ptr;
            // cl define
            platdevices_info *platdevices;

            shared_ptr<util::clutil_logging<std::string, int> >    *logger_ptr;
            util::clutil_logging<std::string, int>    *logger;

            // input open cl
            datastructure::node_tire<std::string, char>   *node_tire_input;
            datastructure::node_tire<std::string, char>   *node_tire_output;

            std::vector<cl_mem> mem_object_vec;

            //array_parallel
            datastructure::absalgorithms<TireDefine, datastructure::datastructure_def::ntire_veclist>   *absalgo;
            datastructure::arrays_parallel<char,TireDefine, datastructure::datastructure_def::ntire_veclist, WorkTypes> * array_pl;
            char *sig_input;
            node_data *node;
    };

    template<typename WorkTypes,typename TireDefine>
    memory<WorkTypes, TireDefine>::memory()
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
    template<typename WorkTypes,typename TireDefine>
    bool memory<WorkTypes, TireDefine>::cl_create_buffer(WorkTypes& worktype_loads,
            datastructure::absalgorithms<TireDefine,
            datastructure::datastructure_def::ntire_veclist>& abs_tireall)
    {
        logger->write_info("### Start cl_create_buffer ###", util::format_type::type_header);
        absalgo = &abs_tireall;
				array_pl = static_cast<datastructure::arrays_parallel<char,TireDefine, datastructure::datastructure_def::ntire_veclist, WorkTypes> * >(absalgo);
				array_pl->init_workload(worktype_loads, *absalgo->get_containdata());
				
        //build_node_buffer_object(platdevices, node, count_sig_input);

        return true;
    }


    /**
    * @brief Buffer for node struct
    *
    * @param plat_info  Platform devices struct class
    * @param node  Node of struct
    */
    template<typename WorkTypes,typename TireDefine>
    void memory<WorkTypes, TireDefine>::build_node_buffer_object(platdevices_info *plat_info, node_data   *node, int count_sig_input)throw(std::runtime_error)
    {
        cl_int err = CL_SUCCESS;
        int node_size = 0;
        logger->write_info("Node size insert ", lexical_cast<std::string>(sizeof(node)));
        logger->write_info("-- Node all size    ", lexical_cast<std::string>(count_sig_input));
        logger->write_info("-- size of Node data    ", lexical_cast<std::string>(sizeof(node_data)));

        char *data_str = (char *)malloc( count_sig_input * sizeof(char));

        for(int count_data = 0; count_data < count_sig_input; count_data++) {
            data_str[count_data] = node->data[count_data];
        }

        try {
            // inital vector size
            plat_info->vec_buffer.initial_size(6);
            //Node input
            cl_mem  buffer_input = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                    sizeof(node_data) * (count_sig_input),
                    node,
                    &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            // Node->data initial root and child
            plat_info->vec_buffer.push_back(buffer_input);

            cl_mem  sigrc_input = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                    sizeof(char) * (count_sig_input),
                    &data_str,
                    &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            // set size of buffer input
            // Work ID, Set default 0
            plat_info->vec_buffer.push_back(sigrc_input);

            // signature size
            cl_mem  buffer_input_sizes = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                    sizeof(int),
                    &count_sig_input,
                    &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            // set size of buffer input
            // Work ID, Set default 0
            plat_info->mem_input_buffers_sizes = count_sig_input;
            plat_info->vec_buffer.push_back(buffer_input_sizes);

            plat_info->work_dim = new unsigned int[1];
            plat_info->work_dim[0] = 5;
            cl_mem  work_dim_value = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                    sizeof(uint),
                    &plat_info->work_dim,
                    &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            plat_info->vec_buffer.push_back(work_dim_value);

            // Global ID, Set default 0
            plat_info->global_id = new unsigned int[1];
            plat_info->global_id[0] = 6;
            cl_mem  global_id_value = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                    sizeof(uint),
                    &plat_info->global_id,
                    &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            plat_info->vec_buffer.push_back(global_id_value);

            // Add unsigned char to devices
            cl_mem  sigrc_output = clCreateBuffer(
                    plat_info->context,
                    CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                    sizeof(char) * count_sig_input,
                    &plat_info->global_id,
                    &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateBuffer-Build-node-Buffer");

            plat_info->vec_buffer.push_back(sigrc_output);

        } catch(std::runtime_error&  ex) {
            logger->write_error( ex.what() );
        }

    }

    /**
    * @brief Releases memory in buffer.
    *
    * @return True, Memory releases successe. False, Memory releases unsuccess and throws exceptions.
    */
    template<typename WorkTypes, typename TireDefine>
    bool memory<WorkTypes, TireDefine>::cl_release_memory()
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

    template<typename WorkTypes, typename TireDefine>
    bool memory<WorkTypes, TireDefine>::cl_create_subbuffer()
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
    template<typename WorkTypes, typename TireDefine>
    bool memory<WorkTypes, TireDefine>::cl_check_buffer_size()
    {
        logger->write_info("### Start cl_check buffer size ###",util::format_type::type_header);
        platdevices_info *platdevices = get_platdevices_data();

        int count_mem_object = 0;
        int sum_signature = 0;

        char *signature_input;
        signature_input = new char[platdevices->node_tire_input->size()];

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
    template<typename WorkTypes,
             typename TireDefine = datastructure::tire<std::string,
             char,
             std::list<std::vector<datastructure::node_tire<std::string, char> > >,
             datastructure::worktypes<int> > >
    class clutil_memory : public base_clutil
    {
        public:
            clutil_memory();

            void cl_create_buffer(WorkTypes& worktype_loads,
                    datastructure::absalgorithms<TireDefine, datastructure::datastructure_def::ntire_veclist>& buffer_vec_str);
            bool cl_create_subbuffer();
            bool cl_check_buffer_size();

            bool cl_release_memory();

            ~clutil_memory();

            std::vector<shared_ptr<platdevices_info> >&    get_platdevices_info_vec() {
                return  memory_util->get_platdevices_info_vec();
            }
            void set_platdevices_info_vec(std::vector<shared_ptr<platdevices_info> >& ptr_info) {
                memory_util->set_platdevices_info_vec(ptr_info);
            }

            platdevices_info *get_platdevices_data() {
                return memory_util->get_platdevices_data();
            }

        private:
            memory<WorkTypes, TireDefine> *memory_util;

    };

    template<typename WorkTypes,typename TireDefine>
    clutil_memory<WorkTypes, TireDefine>::clutil_memory()
    {
        memory_util = new memory<TireDefine>();
    }

    template<typename WorkTypes,typename TireDefine>
    bool clutil_memory<WorkTypes, TireDefine>::cl_release_memory()
    {
        return memory_util->cl_release_memory();
    }

    template<typename WorkTypes,typename TireDefine>
    void  clutil_memory<WorkTypes,TireDefine>::cl_create_buffer(WorkTypes& worktype_loads,
            datastructure::absalgorithms<TireDefine, datastructure::datastructure_def::ntire_veclist>& buffer_vec_str)
    {
        memory_util->cl_create_buffer(worktype_loads,buffer_vec_str);
    }

    template<typename WorkTypes,typename TireDefine>
    bool  clutil_memory<WorkTypes, TireDefine>::cl_create_subbuffer()
    {
        return	memory_util->cl_create_subbuffer();
    }

    template<typename WorkTypes,typename TireDefine>
    bool clutil_memory<WorkTypes, TireDefine>::cl_check_buffer_size()
    {
        return memory_util->cl_check_buffer_size();
    }

    template<typename WorkTypes,typename TireDefine>
    clutil_memory<WorkTypes, TireDefine>::~clutil_memory()
    {

    }

}

#endif /*HNMAV_KERNEL_CLUTIL_HPP */