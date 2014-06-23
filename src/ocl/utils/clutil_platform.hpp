#ifndef HNMAV_KERNEL_CLUTIL_PLATFORM_HPP
#define HNMAV_KERNEL_CLUTIL_PLATFORM_HPP

/*                       Titles                                          Authors              Date
 *-Initial utility infomation                                            Chatsiri.rat				  19/08/2012
 *-Add for info device, exception                                        Chatsiri.rat					20/08/2012
 *-Add  share_ptr for handling malloc memory
 *                                                                       Chatsiri.rat					25/08/2012
 *-Base_cltuil new cl_info object                                        Chatsiri.rat	        01/09/2012
 *-Change buffer_elemtns to struct buffer_elements
 *                                                                       Chatsiri.rat				  14/09/2012
 */

//BOOST
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

//OpenCL
//#include "CL/cl.h"

//STD
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <alloca.h>
#include <malloc.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

//INTERNAL
#include "utils/logger/clutil_logger.hpp"
#include "exception/system_exception.hpp"
#include "ocl/utils/clutil_infodevice.hpp"
#include "ocl/utils/base_clutil.hpp"

#define DEFAULT_PLATFORM 0
#define DEFAULT_USE_MAP  false
#define NUM_BUFFER_ELEMENTS 16
#define SIZE_PROPERTIES 2;

#define MAX_SOURCE_SIZE (0x100000)

namespace hnmav_kernel
{

    using namespace boost;
    using namespace hnmav_util;
    using namespace hnmav_exception;
		/*
    void CL_CALLBACK contextCallback(
            const char *errInfo,
            const void *private_info,
            size_t cb,
            void *user_data)
    {
        std::cout << "Error occured during context use: " << errInfo << std::endl;
    }
		*/

    /* initial Real class */
    class platform : public clutil
    {
        public:


            platform(): platform_id(DEFAULT_PLATFORM), use_map(DEFAULT_USE_MAP) {
                logger_ptr = &clutil_logging<std::string, int>::get_instance();
                logger = logger_ptr->get();
            }

            void set_num_platforms_ids(cl_uint number_entries,
                    cl_platform_id *platforms);

            bool input_kernel_source(std::string str);

            std::string&   get_source();
            std::size_t get_source_size();

            bool init_platforms_ids();
            std::string get_platforms_info(cl_platform_info name, std::string str);
            cl_platform_id get_platform_ids();


            void cl_get_devices_info();
            cl_device_id& get_device_ids(cl_device_type device_type);

            void  context_properties();
            void cl_create_context();

            void cl_create_programs_with_source();
            void cl_build_programs();

            // release programs after run
            bool cl_release_program();
            bool cl_release_context();

            ~platform();

            std::vector<shared_ptr<platdevices_info> >&    get_info() {
                return get_platdevices_info_vec();
            }

        private:

            std::ifstream    *input_file_stream;
            std::string      *src_prog;
            std::string      *info;
            std::string      src;

            int platform_id;
            bool use_map;

            shared_ptr<clutil_logging<std::string, int> > *logger_ptr;
            clutil_logging<std::string, int>   *logger;

            FILE *fp;
            char *source_str;
            std::size_t source_size;

    };


    class clutil_platform : public clutil
    {
        public:
            clutil_platform(): platform_id(DEFAULT_PLATFORM), use_map(DEFAULT_USE_MAP)
                , platform_util(new platform()) {

            }


            std::string get_platforms_info(cl_platform_info name, std::string str);
            void set_num_platforms_ids(cl_uint number_entries,cl_platform_id *platforms);
            bool init_platforms_ids();
            cl_platform_id get_platform_ids();

            bool           input_kernel_source(std::string str);
            std::string&   get_source();
            std::size_t    get_source_size();
            cl_device_id& get_device_ids(cl_device_type device_type);
            void           cl_get_devices_info();



            void  context_properties();
            void cl_create_context();

            void cl_create_programs_with_source();
            void cl_build_programs();
            ~clutil_platform();

            bool cl_release_context();
            bool cl_release_program();

            std::vector<shared_ptr<platdevices_info> >&   get_info() {
                return platform_util->get_info();
            }

        private:

            int platform_id;
            bool use_map;
            platform *platform_util;

    };



}
#endif /* HNMAV_KERNEL_CLUTIL_PLATFORM_HPP  */

