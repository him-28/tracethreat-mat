#ifndef UTILS_BASE_COMMON_HPP
#define UTILS_BASE_COMMON_HPP
/* 						Titles														Authors 												Date
 * -Common check programming                    R.Chatsiri                      10/02/2014
 */

#include <stdlib.h>
#ifndef _MSC_VER
#include "stdint.h"   //Linux standard header.
#else
#include "utils/base/stdint_msvc.hpp" //VCpp supported header.
#endif

namespace utils
{
    /**
    * @brief Plugins for cirtical file scanning.
    */
    struct plugins_result {
        uint8_t  status;
        bool     infected_status;
    };

    // Set result in struct before send to connector class.
    /**
    * @brief Result after matched on OCL and TBB module.
    */
    template<typename MAPPED_FILE>
    struct file_scan_result {
        /**
        * @brief
        */
        const char *file_name;
        /**
        * @brief
        */
        size_t       file_size;
        /**
        * @brief
        */
        //   MAPPED_FILE  file_detail;
        uint64_t    start_index;
        uint64_t    end_index; //support TBB
        char *vir_name;  // support TBB
        char *keywords;  // keyword  TBB
    };

}



#endif /* UTILS_BASE_COMMON_HPP */
