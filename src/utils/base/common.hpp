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

#include "internet/msg/scan_server_client/message_scan.pb.h"

namespace utils
{

    using boost::assign::map_list_of;

    //_____________________________________ File system under system __________________________________
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

			  //std::string sig;   // signature full.
        //std::string virname;  // virus name
        //std::string file_name;  // result file name


        //support OCL
        uint8_t  sig_type; // MD5, SHA-1, SHA-256
        uint64_t offset; // offset start search virus
        const char *sig_detail;  // Declare detail of virus

        //support tbb
        char         keyword_str; //Keyword per node.
        uint64_t     keyword_index; // Index of keyword.
        uint32_t     sig_size;
        uint64_t       index_start;
        std::size_t    state;

				//meta_sig(){ }

        meta_sig():
            sig_type(0),
            offset(0),
            state(0) { sig = ""; virname = ""; sig_detail=""; file_name=""; }
	
				meta_sig(int sig_size, int virname_sig) : sig(new char(sig_size)),
				virname(new char(virname_sig)){ }

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

        /**
        * @brief Binary for external message scanning result.( Support to external message )
        */
        const char *binary;

        /**
        * @brief MD5, SHA-256 or SSDEEP types.( Support to external message )
        */
        message_scan::ResponseScan::EncodeType scan_type;

        /**
        * @brief PE , ELF and SHA-256 types.( Support to external message )
        */
        message_scan::ResponseScan::FileType file_type;

    };

    //______________________________ File system over network _____________________________
    /**
    * @brief File detail received from server
    */
    struct file_scan_request {
        //file type
        std::string binary;
        std::string file_name;
        uint64_t    file_size;

        message_scan::RequestScan::EncodeType scan_type;

        message_scan::RequestScan::FileType file_type;

        file_scan_request():
            binary(""),
            file_name(""),
            file_size(0),
            scan_type(message_scan::RequestScan::MD5),
            file_type(message_scan::RequestScan::PE) { }
    };

    /**
    * @brief File detail send to server.
    */
    struct file_scan_response {
        //file type
        std::string binary;
        std::string file_name;
        uint64_t    file_size;

        message_scan::ResponseScan::EncodeType scan_type;

        message_scan::ResponseScan::FileType file_type;

        file_scan_response():
            binary(""),
            file_name(""),
            file_size(0),
            scan_type(message_scan::ResponseScan::MD5),
            file_type(message_scan::ResponseScan::PE) { }
    };

    typedef boost::unordered_map<utils::filetype_code, std::string>
    filetype_code_type;

    static	const std::string  filetype_code_map(utils::filetype_code ftype_code)
    {
        const filetype_code_type  filetype_engine_def  =
                boost::assign::map_list_of(utils::pe_file,"pe_actire_engine")
                (utils::elf_file, "elf_actire_engine");
        return filetype_engine_def.at(ftype_code);
    }

}

#endif /* UTILS_BASE_COMMON_HPP */
