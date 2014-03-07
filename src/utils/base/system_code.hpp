#ifndef UTIL_SYSTEM_CODE_HPP
#define UTIL_SYSTEM_CODE_HPP

// File scanning
#define FILE_FOUND 								0   /* Scan infected file found */
#define FILE_NOT_FOUND 						-1   /* Scan infected file not found */

//OpenCL code
#define KERNEL_NOT_LOAD           -50  /* OpenCL cannot load  .cl extension file */
#define BUFFER_NOT_LOAD						-51  /* KernelArg cannot load buffer */
//Certificate code


//Database connection code



//Network Connection code.

//Sandbox code


//File type


//MapReduce



namespace util
{
    typedef	enum SCAN_FILE_CODE { found = FILE_FOUND ,
            not_found = FILE_NOT_FOUND
                                } scan_file_code;
    typedef enum OCL_SYSTEM_CODE { kernel_not_load = KERNEL_NOT_LOAD,
            buffer_not_LOAD = BUFFER_NOT_LOAD
                                 } ocl_system_code;
}

#endif
