#ifndef HNMAV_KERNEL_CLUTIL_PLATFORM_HPP
#define HNMAV_KERNEL_CLUTIL_PLATFORM_HPP

/*                       Titles                                          Authors                        Date
 *-Initial utility infomation                                            Chatsiri.rat										19/08/2012
 *-Add for info device, exception                                        Chatsiri.rat										20/08/2012
 *-Add  share_ptr for handling malloc memory
 *                                                                       Chatsiri.rat									  25/08/2012
 *-Base_cltuil new cl_info object                                        Chatsiri.rat	                  01/09/2012
 *-Change buffer_elemtns to struct buffer_elements
 *                                                                       Chatsiri.rat										14/09/2012
 */

//BOOST
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

//OpenCL
#include "CL/cl.h"

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
#include "kernels/utils/clutil_infodevice.hpp"

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

    /* platform declare */
    platform::~platform()
    {
        logger->write_info("clutil_platform delete object ");
    }

    cl_platform_id	platform::get_platform_ids()
    {
        return NULL;
    }


    bool platform::cl_release_context()
    {
        platdevices_info   *platdevices = get_platdevices_data();
        clReleaseContext(platdevices->context);
        logger->write_info("--- Release Context Completed");
        return true;
    }

    bool platform::cl_release_program()
    {
        platdevices_info   *platdevices = get_platdevices_data();
        clReleaseProgram(platdevices->program);
        logger->write_info("--- Release Program Completed");
        return true;
    }

    void platform::set_num_platforms_ids(cl_uint number_entries,
            cl_platform_id *platforms)
    {
        try {
            logger->write_info("### Start in num_platforms_ids ###", format_type::type_header);

            platdevices_info   *platdevices = get_platdevices_data();

            logger->write_info("--- Number entires   ", lexical_cast<std::string>(number_entries));
            logger->write_info("--- Platforms number ", lexical_cast<std::string>(platdevices->num_platforms));

            cl_int err = clGetPlatformIDs(number_entries, platforms , &platdevices->num_platforms);

            if((err != CL_SUCCESS)  ? err : (platdevices->num_platforms <= 0 ? -1 : CL_SUCCESS)) {
                throw	cl::clutil_exception(err,"Device Info");
            }

        } catch(std::runtime_error  e) {
            logger->write_info( e.what() );
            exit(EXIT_FAILURE);
        }

    }

    /**
    * @brief Stage-1 clGetPlatformIDs get number of platforms.
    *        Stage-2 clGetPlatformDIS get platform ids
    * @return return true if not error on 'cl_int err'
    */
    bool platform::init_platforms_ids()
    {
        logger->write_info("### Start init_platforms_ids ### ", format_type::type_header);

        try {
            platdevices_info   *platforms = get_platdevices_data();

            cl_int err = clGetPlatformIDs(0, NULL, &platforms->num_platforms);

            if(err != CL_SUCCESS) {
                throw cl::clutil_exception(err, "clGetPlatformIDs");
            }

            platforms->platform_ids = (cl_platform_id *)malloc(sizeof(cl_platform_id) * platforms->num_platforms );

            err |= clGetPlatformIDs(platforms->num_platforms, platforms->platform_ids, NULL);

            logger->write_info("--- Number platform ",lexical_cast<std::string>(platforms->num_platforms));
            logger->write_info("--- Platform IDs    ",lexical_cast<std::string>(&platforms->platform_ids));

            if(err != CL_SUCCESS) {
                throw cl::clutil_exception(err, "clGetPlatformIDs");
            }

            DisplayPlatformInfo(platforms->platform_ids[platform_id], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");

            return true;
        } catch(std::runtime_error e) {
            logger->write_error(e.what());
            exit(EXIT_FAILURE);
        }
    }

    cl_device_id& platform::get_device_ids(cl_device_type device_type)
    {
        logger->write_info("### Start get_devices_ids ###", format_type::type_header);
        platdevices_info   *devices = get_platdevices_data();

        for(int count_platform  = 0; count_platform < devices->num_platforms; count_platform++) {
            try {

                cl_int err = clGetDeviceIDs(
                        devices->platform_ids[count_platform],
                        device_type,
                        0,
                        NULL,
                        &devices->num_devices);

                if(err != CL_SUCCESS && err != CL_DEVICE_NOT_FOUND) {
                    logger->write_info("!!! Cannot initial devices ids-stage 1 ", format_type::type_header);
                    throw   cl::clutil_exception(err, "clGetDeviceIDs");
                } else if(devices->num_devices > 0) {
                    devices->device_ids =  (cl_device_id *)malloc(sizeof(cl_device_id) * devices->num_devices );
                    err = clGetDeviceIDs(
                            devices->platform_ids[count_platform],
                            device_type,
                            devices->num_devices,
                            &devices->device_ids[0],
                            NULL);
                }

                if(*(&devices->device_ids) == NULL) {
                    logger->write_info("!!! Cannot initial devices ids-stage 2 ", format_type::type_header);
                    throw   cl::clutil_exception(err, "clGetDeviceIDs- NO CPU Device found");
                }
            } catch(std::runtime_error e) {

                logger->write_error(e.what());
                exit(EXIT_FAILURE);
            }

            logger->write_info("--- Count Platform number", lexical_cast<std::string>(count_platform));
            logger->write_info("--- Devices number       ", lexical_cast<std::string>(devices->num_devices));
        }

        return *devices->device_ids;

    }

    void platform::context_properties()
    {
        logger->write_info("### Start context_properties ###", format_type::type_header);
        platdevices_info   *platforms = get_platdevices_data();

        platforms->context_vec.push_back(CL_CONTEXT_PLATFORM);
        platforms->context_vec.push_back((cl_context_properties)platforms->platform_ids[platform_id]);
        platforms->context_vec.push_back(0);

    };


    void  platform::cl_create_programs_with_source()
    {
        logger->write_info("### Create create program with source ###", format_type::type_header);
        platdevices_info   *devices = get_platdevices_data();

        try {
            cl_int err = CL_SUCCESS;

            std::string source = get_source();
            const char  *str = source.c_str();
            std::size_t  length = source.length();

            devices->program = clCreateProgramWithSource(devices->context, 1, &str, &length , &err);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clCreateProgramWithSource");

        } catch(std::runtime_error e) {
            logger->write_error(e.what());
        }
    }


    void platform::cl_build_programs()
    {
        logger->write_info("### Create build programs ###", format_type::type_header);

        platdevices_info   *devices = get_platdevices_data();
      
        const char options[] = "-Werror -cl-std=CL1.1 -I /home/chatsiri/workspacecpp/Hanumanav/src/kernels/cl/";

        logger->write_info("--- Device programs ", lexical_cast<std::string>(devices->program));
        logger->write_info("--- Device Number   ", lexical_cast<std::string>(devices->num_devices));
        logger->write_info("--- Device Id       ", lexical_cast<std::string>(devices->device_ids[0]));


        try {
            cl_int err = clBuildProgram(
                    devices->program,
                    devices->num_devices,
                    devices->device_ids, // define array sizes [0]
                    options,
                    NULL,
                    NULL);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err, "clBuildProgram");

        } catch(std::runtime_error e) {
            char build_log[16389];
            clGetProgramBuildInfo(
                    devices->program,
                    *devices->device_ids, // define array sizes [0]
                    CL_PROGRAM_BUILD_LOG,
                    sizeof(build_log),
                    build_log,
                    NULL);

            logger->write_error(e.what());
            logger->write_info( "Build log = ",build_log);
        }

        logger->write_info("--- Create Programs ", lexical_cast<std::string>(devices->program));

    }


    bool platform::input_kernel_source(std::string str)
    {
        logger->write_info("####   Input Kernel Source ####", format_type::type_header);
        logger->write_info("--- Platform input kernel ", str);

        try {
            input_file_stream  = new std::ifstream(str.c_str());

            if(input_file_stream->is_open()) {
                src_prog = new std::string(std::istreambuf_iterator<char>(*input_file_stream),
                        std::istreambuf_iterator<char>());
                return true;
            }

            std::string error_msg_file = "Reading source : ";
            throw cl::clutil_exception(-1,  error_msg_file.append(str) );

        } catch(std::runtime_error e) {
            logger->write_error( e.what() );
            return false;
        }

        return true;
    }

    void CL_CALLBACK contextCallback(
            const char *errInfo,
            const void *private_info,
            size_t cb,
            void *user_data)
    {
        std::cout << "Error occured during context use: " << errInfo << std::endl;
    }


    void  platform::cl_create_context()
    {
        logger->write_info("### Create... cl_create_contxt ###", format_type::type_header);

        platdevices_info   *devices = get_platdevices_data();
        const cl_device_id *devices_id  = devices->device_ids;
        cl_uint             devices_num = devices->num_devices;
        logger->write_info("--- Check properties ", lexical_cast<std::string>(devices->properties));
        logger->write_info("--- Devices Number   ", lexical_cast<std::string>(*&devices_num));
        logger->write_info("--- Devices IDs      ", lexical_cast<std::string>(devices_id));

        cl_context_properties context_pro[] = {
            CL_CONTEXT_PLATFORM,
            (cl_context_properties)devices->platform_ids[platform_id],
            0
        };

        try {
            cl_int err = CL_SUCCESS;

            devices->context =  clCreateContext(
                    context_pro,
                    devices_num,
                    devices_id,
                    contextCallback,
                    NULL,
                    &err );

            if(*(&devices->context) == NULL || err != CL_SUCCESS)
                throw cl::clutil_exception(err, "Cannot create Context");

            logger->write_info("--- context address ", lexical_cast<std::string>(devices->context));

        } catch(std::runtime_error e) {
            logger->write_error( e.what() );
        }

    }


    std::string platform::get_platforms_info(cl_platform_info  name, std::string str)
    {
        logger->write_info( "### Start get_platforms_info ###", format_type::type_header );
        std::size_t param_value_size;
        std::string info_str;

        try {
            platdevices_info   *platforms = get_platdevices_data();

            logger->write_info("--- Platform IDs   ", lexical_cast<std::string>(platform_id));
            logger->write_info("--- Platform value ", lexical_cast<std::string>(platforms->platform_ids[platform_id]));

            cl_int err = clGetPlatformInfo(
                    platforms->platform_ids[platform_id],
                    name,
                    0,
                    NULL,
                    &param_value_size);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err,"Fail to fine OpenCL Platform stage 1");

            char *info = (char *)alloca(sizeof(char) * param_value_size);
            err = clGetPlatformInfo(
                    *platforms->platform_ids,
                    name,
                    param_value_size,
                    info,
                    NULL);

            if(err != CL_SUCCESS)
                throw cl::clutil_exception(err,"Fail to fine OpenCL Platform stage 2");

            logger->write_info("--- Platform information ",  std::string(info));

        } catch(std::runtime_error e) {
            logger->write_error(e.what());
            exit(EXIT_FAILURE);

        }

        return info_str;
    }

    void platform::cl_get_devices_info()
    {
        logger->write_info("#### Start cl_get_devices_inf ####", format_type::type_header);

        platdevices_info   *platforms = get_platdevices_data();
        clGetDeviceInfo(
                *(platforms->device_ids),
                CL_DEVICE_MAX_COMPUTE_UNITS,
                sizeof(platforms->global_size),
                &platforms->global_size,
                NULL);

        platforms->global_size = 100;

        logger->write_info("--- Platform global size ", lexical_cast<std::string>(platforms->global_size));

        clGetDeviceInfo(
                *(platforms->device_ids),
                CL_DEVICE_MAX_WORK_GROUP_SIZE,
                sizeof(platforms->local_size),
                &platforms->local_size,
                NULL);

        platforms->local_size = 50;

        logger->write_info("---  Platform local size ", lexical_cast<std::string>(platforms->local_size));


    }

    std::string& platform::get_source()
    {
        return  *src_prog;
    }

    std::size_t platform::get_source_size()
    {
        return src_prog->length();
    }



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

    /* clutil_platform declear */
    void clutil_platform::set_num_platforms_ids(cl_uint number_entries,
            cl_platform_id *platforms)
    {

        platform_util->set_num_platforms_ids(number_entries, platforms);
    }

    bool  clutil_platform::cl_release_context()
    {

        return platform_util->cl_release_context();

    }

    bool  clutil_platform::cl_release_program()
    {
        return platform_util->cl_release_program();
    }


    bool  clutil_platform::init_platforms_ids()
    {
        return platform_util->init_platforms_ids();
    }

    bool clutil_platform::input_kernel_source(std::string str)
    {
        return platform_util->input_kernel_source(str);
    }

    std::string&   clutil_platform::get_source()
    {
        return platform_util->get_source();
    }

    std::size_t clutil_platform::get_source_size()
    {
        return platform_util->get_source_size();
    }

    std::string clutil_platform::get_platforms_info(cl_platform_info name, std::string str)
    {
        return platform_util->get_platforms_info(name, str);
    }

    cl_device_id& clutil_platform::get_device_ids(cl_device_type device_type)
    {
        return platform_util->get_device_ids(device_type);
    }

    cl_platform_id  clutil_platform::get_platform_ids()
    {
        return platform_util->get_platform_ids();
    }

    void  clutil_platform::context_properties()
    {
        platform_util->context_properties();
    }

    void clutil_platform::cl_create_context()
    {
        platform_util->cl_create_context();
    }

    void clutil_platform::cl_create_programs_with_source()
    {
        platform_util->cl_create_programs_with_source();
    }

    void clutil_platform::cl_get_devices_info()
    {
        platform_util->cl_get_devices_info();
    }

    void clutil_platform::cl_build_programs()
    {
        platform_util->cl_build_programs();
    }

    clutil_platform::~clutil_platform()
    {
    }

}
#endif /* HNMAV_KERNEL_CLUTIL_PLATFORM_HPP  */

