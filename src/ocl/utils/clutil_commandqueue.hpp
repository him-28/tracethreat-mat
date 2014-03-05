#ifndef  HNMAV_KERNEL_CLUTIL_COMMANDQUEUE_HPP
#define  HNMAV_KERNEL_CLUTIL_COMMANDQUEUE_HPP

// name of buildtire kernel
//#define  BUILD_TIRE_KERNEL "buildtire"

#define  BUILD_TIRE_KERNEL "actire_search"

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

//#include "ocl/cl_bootstrap.hpp"
#include "ocl/utils/base_clutil.hpp"
#include "utils/logger/clutil_logger.hpp"

#include "exception/system_exception.hpp"
#include "exception/datastructure_exception.hpp"

#include "clutil_infodevice.hpp"
#include <vector>


namespace hnmav_kernel
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
            bool cl_write_event();
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


    class clutil_commandqueue : public base_clutil
    {
        public:
            clutil_commandqueue() : commandqueue_util(new commandqueue()) { }

            bool cl_create_command_queue();
            bool cl_write_event();
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

}



#endif /* HNMAV_KERNEL_CLUTIL_COMMANDQUEUE_HPP */
