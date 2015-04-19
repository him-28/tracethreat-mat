#ifndef  KERNEL_CLUTIL_COMMANDQUEUE_HPP
#define  KERNEL_CLUTIL_COMMANDQUEUE_HPP

/*                       Titles                                          Authors           		Date
 *-Support command queue (Interface send/receive with vector<shared_ptr<platdevices_info> >
 *                                                                       Chatsiri.rat         07/09/2012
 *-Add input_str(Experimental with std::string). input_str is private variable
 * ( not plan include in struct name platdevices_info. )
 *                                                                       Chatsiri.rat         09/09/2012
 *-Add  memory buffer input & output member function.
 *                                                                       Chatsiri.rat         11/09/2012
 */

#define  BUILD_TIRE_KERNEL "actire_search"

#define SEARCH_BYTES_PER_ITEM 512
#define LOCAL_SIZE            256

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <vector>

//#include "ocl/cl_bootstrap.hpp"
#include "ocl/utils/base_clutil.hpp"
#include "utils/logger/clutil_logger.hpp"

#include "exception/system_exception.hpp"
#include "exception/datastructure_exception.hpp"

#include "clutil_infodevice.hpp"
#include "utils/base/timer_queue.hpp"

namespace kernel
{
    using namespace boost;
    using namespace utils;
    using namespace trace_exception;
    using utils::timer_queue;

    class commandqueue : public base_clutil
    {
        public:

            commandqueue() {
            }

            bool cl_create_command_queue();
            bool cl_write_event();

            bool cl_create_kernel();

            bool cl_release_kernel();
            bool cl_release_commandqueue();

            bool cl_enqueue_task();
            bool cl_enqueue_nd_task(std::vector<uint8_t> *result_vec);

            bool cl_enqueue_copy_buffer();
            bool cl_enqueue_map_buffer();

            bool cl_enqueue_map_buffer(cl_mem device_buffer,
                    void  *&host_ptr,
                    size_t size_bytes,
                    cl_map_flags flags);


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

            ~commandqueue();

        private:

            std::size_t *buffer_elements;
            std::vector<shared_ptr<platdevices_info> >  platdevices_shared_ptr;
            // cl define
            platdevices_info *platdevices;
            void   *mapped_memory;

            // logging
            shared_ptr<clutil_logging<std::string, int> > *logger_ptr;
            clutil_logging<std::string, int>   *logger;
    };


    class clutil_commandqueue : public base_clutil
    {
        public:
            clutil_commandqueue() : commandqueue_util(new commandqueue()) { }

            bool cl_create_command_queue();
            bool cl_write_event();
            bool cl_create_kernel();

            bool cl_enqueue_task();
            bool cl_enqueue_nd_task(std::vector<uint8_t> *result_vec);

            bool cl_enqueue_copy_buffer();
            bool cl_enqueue_map_buffer();

            bool cl_enqueue_map_buffer(cl_mem device_buffer,
                    void  *&host_ptr,
                    size_t size_bytes,
                    cl_map_flags flags);

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

            ~clutil_commandqueue();

        private:
            commandqueue *commandqueue_util;
    };

}



#endif /* HNMAV_KERNEL_CLUTIL_COMMANDQUEUE_HPP */
