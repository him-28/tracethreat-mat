#ifndef PE_FILE_CONTROLLER__HPP
#define PE_FILE_CONTROLLER__HPP



#define EC16(x) ((uint16_t)convert_ec16(&(x)))
#define EC32(x) ((uint32_t)convert_ec32(&(x)))


#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <list>


#include "filetypes/pe.hpp"

//
#include "utils/logger/clutil_logger.hpp"
// Big endian supported type.
union unaligned_64 {
    uint64_t una_u64;
    int64_t una_s64;
} __attribute__((packed));

union unaligned_32 {
    uint32_t una_u32;
    int32_t una_s32;
} __attribute__((packed));

union unaligned_16 {
    uint16_t una_u16;
    int16_t una_s16;
} __attribute__((packed));

namespace filetypes
{

    namespace h_util = hnmav_util;

    template<typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class pe_file_controller
    {

        public:
						pe_file_controller();
            /**
            * @brief Get PE Header file from file system.
            *
            * @param mapped_file_vec  Insert file detail from memory mapped.
            *
            * @return Vector contains header file of PE.
            */
            boost::shared_ptr<std::vector<IMAGE_NT_HEADERS *> >&
            get_pe_header(std::vector<MAPPED_FILE *> *mapped_file_vec);

            /**
            * @brief List PE header detail
            *
            * @param image_nt_header  Struct of PE header.
            *
            * @return String contain detail of PE header
            */
            std::list<std::string> list_pe_header(struct IMAGE_NT_HEADERS *image_nt_header);

            /**
            * @brief File checked Magic , File DLL, Executable
            *
            * @param image_nt_header Struct of PE header
            *
            * @return type check.
            */
            uint8_t check_header_type(struct IMAGE_NT_HEADERS *image_nt_header);

            /**
            * @brief EC16 convert buffer uint8_t to int16_t.
            *
            * @param buffer  input from pe file.
            *
            * @return  int16_t type.
            */
            inline int16_t convert_ec16(uint16_t *buffer);
            /**
            * @brief EC32 convert buffer uint8_t to int32_t
            *
            * @param buffer  input from pe file.
            *
            * @return int32_t type
            */
            inline int32_t convert_ec32(uint16_t *buffer);

        private:
            IMAGE_NT_HEADERS *image_nt_header;

            std::vector<boost::shared_ptr<std::vector<IMAGE_NT_HEADERS *> > > pe_header_vec_shared;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

}


#endif
