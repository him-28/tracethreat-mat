#ifndef UTILS_SYSTEM_CODE_HPP
#define UTILS_SYSTEM_CODE_HPP

/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                                     Authors	          Date
 * - Scanning file system.                                        R.Chatsiri      08/04/2014
 */

// File scanning
#define INFECTED_FOUND 								0   /* Scan infected file found */
#define INFECTED_NOT_FOUND 						-1   /* Scan infected file not found */

// system scanning
#define NEXT_SCANNING                -20 /* Scanning next method */
#define FILE_ERROR                   -21 /* File input error check */

//OpenCL code
#define KERNEL_NOT_LOAD           -50  /* OpenCL cannot load  .cl extension file */
#define BUFFER_NOT_LOAD						-51  /* KernelArg cannot load buffer */
//Certificate code


//Database connection code

//Network Connection code.

//Sandbox code

//File type

//MapReduce



namespace utils
{
    typedef	enum SCAN_FILE_CODE {
        infected_found = INFECTED_FOUND ,
        infected_not_found = INFECTED_NOT_FOUND
    } scan_file_code;

    typedef enum SCAN_FILE_SYSTEM_CODE {
        next_scan  = NEXT_SCANNING,
        file_error = FILE_ERROR
    } scan_file_system_code;

    typedef enum OCL_SYSTEM_CODE {
        kernel_not_load = KERNEL_NOT_LOAD,
        buffer_not_LOAD = BUFFER_NOT_LOAD
    } ocl_system_code;
}

#endif
