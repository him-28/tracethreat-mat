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
#define INFECTED_NOT_FOUND 						0   /* Scan infected file not found */
#define INFECTED_FOUND 								1   /* Scan infected file found */
#define INFECTED_FIRST_STEP         	2   /* Except is infected file. */

// system scanning
#define NEXT_SCANNING                	20 /* Scanning next method */
#define FILE_ERROR                   	21 /* File input error check */

//OpenCL code
#define KERNEL_NOT_LOAD           		50  /* OpenCL cannot load  .cl extension file */
#define BUFFER_NOT_LOAD								51  /* KernelArg cannot load buffer */

// File Type
#define FILETYPE_PE               		100 /* File type PE */
#define FILETYPE_ELF              		101 /* File type ELF */

//Scanning mode
#define MULTIPLE_OCL_MODE         		150 /* Run OCL Mode */
#define MULTIPLE_TBB_MODE         		151 /* Thread Building Block(TBB) Mode */
#define MULTIPLE_OCL_TBB_MODE     		152 /* OCL and TBB mix mode. Load balance concept. */

//Internal and External Message type.
#define INTERNAL_MSG                  160
#define EXTERNAL_MSG                  161

//______________________ Number not depend on consequenctial number _________________________
//File size
#define FILESIZE_MD5     							32  /* MD5 size */ 
#define FILESIZE_SHA1    							32  /* SHA-1 size */
#define FILESIZE_SHA256  							64  /* SHA-256 size */


namespace utils
{
    typedef	enum SCAN_FILE_CODE {
        infected_found = INFECTED_FOUND ,
        infected_not_found = INFECTED_NOT_FOUND,
        infected_fist_step = INFECTED_FIRST_STEP
    } scan_file_code;

    typedef enum SCAN_FILE_SYSTEM_CODE {
        next_scan  = NEXT_SCANNING,
        file_error = FILE_ERROR
    } scan_file_system_code;

    typedef enum OCL_SYSTEM_CODE {
        kernel_not_load = KERNEL_NOT_LOAD,
        buffer_not_LOAD = BUFFER_NOT_LOAD
    } ocl_system_code;

    typedef enum FILETYPE {
        pe_file  = FILETYPE_PE,
        elf_file = FILETYPE_ELF
    } filetype_code;

    typedef enum SIGTYPE_SIZE {
        filesize_md5    = FILESIZE_MD5,
        filesize_sha1   = FILESIZE_SHA1,
        fileIsze_sha256 =  FILESIZE_SHA256
    } sigtype_size;


		typedef enum MSG_TYPE{
			 internal_msg = INTERNAL_MSG,
			 external_msg = EXTERNAL_MSG
		}msg_type;

    typedef enum SCANNING_MODE{
        multiple_ocl_mode = MULTIPLE_OCL_MODE,
        multiple_tbb_mode = MULTIPLE_TBB_MODE,
        multiple_ocl_tbb_mode = MULTIPLE_OCL_TBB_MODE
    } scanning_mode;


}

#endif
