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

#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>

#include "utils/base/system_code.hpp"


namespace utils
{

		using boost::assign::map_list_of;

    /**
    * @brief Plugins for cirtical file scanning.
    */
    struct plugins_result {
        uint8_t  status;
        bool     infected_status;
    };

    /**
    * @brief Meta signature support internal shared_memory(Plan-00004).
    * In Plan-00003 programs use for create signature node for TBBand verfify signature on OCL scanning.
    *
    */
    struct meta_sig {
        //support OCL and TBB
        const char *sig;   // signature full.
        const char *virname;  // virus name
        const char *file_name;  // result file name

        //support OCL
        uint8_t  sig_type; // MD5, SHA-1, SHA-256
        uint64_t offset; // offset start search virus
        const char *sig_detail;  // Declare detail of virus

        //support tbb
        char         keyword_str; //Keyword per node.
        uint32_t     sig_size;
        uint64_t       index_start;
        std::size_t    state;

        meta_sig():
            sig_type(0),
            offset(0),
            sig(""),
            virname(""),
            sig_detail("") { }

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
        size_t     file_size;

        /**
        * @brief Start Index found infected file.
        */
        size_t index_start;

        /**
        * @brief End index found infected file.
        */
        size_t index_end;

        /**
        * @brief Virus name after found on node_str.
        */
        const char *vir_name;  // support TBB

        /**
        * @brief Signature found.(Key of db_name : )
        */
        const char *sig_node;  // keyword  TBB
    };




    typedef boost::unordered_map<utils::filetype_code, std::string>  filetype_code_type;
    static	const std::string  filetype_code_map(utils::filetype_code ftype_code)
    {
        const filetype_code_type  filetype_engine_def  =
                boost::assign::map_list_of(utils::pe_file,"pe_actire_engine")
                (utils::elf_file, "elf_actire_engine");
        return filetype_engine_def.at(ftype_code);
    }

}

#endif /* UTILS_BASE_COMMON_HPP */
