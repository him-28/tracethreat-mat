#ifndef HNMAV_KERNEL_CLUTIL_HPP
#define HNMAV_KERNEL_CLUTIL_HPP

/*                       Titles                                          Authors              Date
 *-interface combine with  platdevices_info( contrain cl infomation )
 *                                                                       Chatsiri.rat         27/08/2012
 *-Add sturct to implement on interface class                            Chatsiri.rat         27/08/2012
 *-Change interface to base prefix name                                  Chatsiri.rat         28/08/2012
 *-Add share_ptr_info base class                                         Chatsiri.rat         30/08/2012
 *-Add Buffer elements                                                   Chatsiri.rat         14/09/2012
 */

//BOOST
#include <boost/shared_ptr.hpp>
//OpenCL
#include "CL/cl.h"
//STL
#include <vector>
#include <iostream>
//Internal
//#include "data_structure/tire.hpp"
//#include "kernels/cl/struct_tire.h"

//- Data structure
#include "data_structure/vector.hpp"

namespace hnmav_kernel
{
    using namespace boost;
    namespace dstructure  = data_structure;

    struct platdevices_info {

        // platform info
        cl_platform_id        *platform_ids;
        cl_device_id          *device_ids;

        cl_context      context;
        cl_context_properties *proper[3];
        cl_context_properties *properties;
        std::vector<cl_context_properties>  context_vec;

        cl_uint  				num_platforms;
        cl_uint					num_devices;
        cl_program 			program;

        std::vector<cl_command_queue> queues;
        std::vector<cl_kernel>        kernels;
        std::vector<cl_event>         events;

        //max size local and global
        std::size_t     global_size; //[3];
        std::size_t     local_size;  //[3];

        // memory info, Memory Type intial detail  from  clCreateBuffer
        // Memory type for send to kernel arguement.
        std::vector<cl_mem>    *mem_input_buffers;
        // Size of buffer node_data insert to devices
        int                     mem_input_buffers_sizes;
				// Vector contains buffer
        dstructure::vector_buffer<cl_mem> vec_buffer;

        // Work ID of argument send-received from kernel
				unsigned int * work_dim;
        // Global ID of argument send-received from kernel
        unsigned int * global_id;
				
        std::vector<cl_mem> 	 *mem_copy_buffers;
        //std::vector<char> *node_tire_input;
        std::vector<char>  				node_symbol_vec;
			  std::vector<size_t>       node_state_vec;
				std::vector<uint8_t>  node_binary_vec;

				//return symbol to host(write back)
				char *      symbol_wb;

        // buffer global size
        int					 buffer_elements;
    };

    class base_clutil
    {
        public:
            /* using shared_ptr_info connect to another classes */
            virtual std::vector<shared_ptr<platdevices_info> >&    get_platdevices_info_vec() = 0;
            virtual void set_platdevices_info_vec(std::vector<shared_ptr<platdevices_info> >& ptr_info) = 0;
            virtual platdevices_info *get_platdevices_data() = 0;
    };

    class clutil : public  base_clutil
    {
        public:
            // interface member function
            std::vector<shared_ptr<platdevices_info> >&   get_platdevices_info_vec() {
                if( platform_vec.size() == 0) {
                    platform_ptr  info_ptr(new platdevices_info);
                    platform_vec.push_back(info_ptr);
                    std::cout<<"### Shared_ptr Init paltdevices_info  = "<< platform_vec.size() << std::endl;
                }

                return  platform_vec;
            }

            // using pass by references to set data in shared_ptr_info
            void set_platdevices_info_vec(std::vector<shared_ptr<platdevices_info> >&    ptr_info) {
                if(platform_vec.back() != NULL) {
                    platform_vec.pop_back();
                    platform_vec.push_back(ptr_info.back());
                }

                return platform_vec.push_back(ptr_info.back());
            }

            platdevices_info *get_platdevices_data() {
                shared_ptr<platdevices_info>   plat_shared_ptr =  platform_vec.back();
                platdevices_info *platdevices =  plat_shared_ptr.get();
                return platdevices;
            }


            virtual std::vector<shared_ptr<platdevices_info> >&   get_info() = 0;

        private:

            typedef	shared_ptr<platdevices_info>        platform_ptr;
            std::vector<shared_ptr<platdevices_info> >  platform_vec;
    };

}

#endif /*HNMAV_KERNEL_CLUTIL_HPP */
