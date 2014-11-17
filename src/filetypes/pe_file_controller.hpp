#ifndef FILETYPES_PE_FILE_CONTROLLER__HPP
#define FILETYPES_PE_FILE_CONTROLLER__HPP



#define EC16(x) ((uint16_t)convert_ec16(&(x)))
#define EC32(x) ((uint32_t)convert_ec32(&(x)))

#define MIN(x,y) ((x < y)?(x):(y))

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <list>


#include "filetypes/pe.hpp"

#include "ocl/cl_bootstrap.hpp"

#include "utils/logger/clutil_logger.hpp"

#include "utils/base/common.hpp"
#include "utils/uuid_generator.hpp"

#include "memory/file_shm_handler.hpp"

#include "threadsyncocl/thread_sync.hpp"

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"

#include "threadconcurrency/tbbpostscan_pe_controller.hpp"

#include "msg/message_tracethreat.pb.h"

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

namespace controller
{
    template<typename MAPPED_FILE>
    struct data_ocl_process;

    template<typename Buffer, typename MAPPED_FILE>
    class  BufferSync;

    template<typename BufferSync, typename MAPPED_FILE>
    class thread_sync;
}

namespace filetypes
{
    template<typename MAPPED_FILE>
    class pe_file_controller;
}

namespace filetypes
{

		
    namespace h_util = hnmav_util;
    namespace dstr   = data_structure;
    namespace kernel_ocl = hnmav_kernel;
		
    using memory::signature_shm;
		using utils::uuid_generator;

    template<typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class pe_file_controller
    {

        public:
            //ocl support
            typedef kernel_ocl::cl_load_system<kernel_ocl::clutil_platform,
                    dstr::dstr_def::work_groupitems,
                    std::vector<boost::unordered_map<char, size_t> >,
                    dstr::actire_parallel<char,
                    size_t,
                    boost::unordered_map<char, size_t>,
                    std::vector<boost::unordered_map<char, size_t> > >
                    >	 load_ocl_system_type;

            typedef memory::signature_shm<struct memory::meta_sig, struct memory::meta_sig_mem>
            				signature_shm_type;

            typedef tbbscan::actire_sig_engine<char, tbbscan::tbb_allocator>  signature_engine_type;

						/*
            typedef memory::signature_shm_pe_controller<struct memory::meta_sig, struct memory::meta_sig_mem>            				 signature_shm_type;
						*/
            typedef tbbscan::iactire_engine<char, tbbscan::tbb_allocator>
            				iactire_engine_scanner_type;

						typedef boost::shared_ptr<message_tracethreat::InfectedFileInfo>  threatinfo_ptr_type;

            pe_file_controller();

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

						//utils::scan_file_code 
            threatinfo_ptr_type scan(
										std::vector<MAPPED_FILE *> *mapped_file_pe,
                    signature_shm_type  *sig_shm,
                    signature_engine_type *sig_engine,
                    iactire_engine_scanner_type   *iactire_engine_scanner);


            /**
            * @brief Insert signature and thread_sync for compute with GPGPU.
            *
            * @param symbol_vec Symbol vector
            * @param state_vec  State vector
            * @param tsync   thread sync file with GPGPU. Start() member function calls in
            *  pe_file_controller.scan() member function.
            * @param ocl_load_system   OCL system object send to slot_ocl_thread.
            *
            * @return Vector contain end result.
            */
            utils::scan_file_code scan(std::vector<char> *symbol_vec,
                    std::vector<size_t> *state_vec,
                    std::vector<MAPPED_FILE *> *mapped_file_pe_vec,
                    std::string *kernel_file_path_ptr,
                    signature_shm_type *sig_shm);


            /**
            * @brief Set opencl to load_system is opencl object
            *
            * @param file_path_kernel  is path of .cl file
            *
            * @return True, if variable contain file size more size than 0.
            */
            bool set_opencl_file(std::string& kernel_file_path);


        private:
            IMAGE_NT_HEADERS *image_nt_header;

            //kernel file path
            std::string *kernel_file_path_ptr;

            //set file_shm
            memory::file_shm_handler<MAPPED_FILE>  f_shm_handler;

						utils::uuid_generator uuid_gen;

            //Load TBB
            controller::tbbpostscan_pe_controller<controller::
            BufferSyncTBB<struct controller::data_tbb_process<struct MAPPED_FILE_PE>,
                        struct MAPPED_FILE_PE>,
                            struct MAPPED_FILE_PE,
                                struct utils::meta_sig> tbbpostscan_pe_col;

            //Load OCL
            typedef controller::thread_sync<controller::BufferSync<
            struct controller::data_ocl_process<MAPPED_FILE>,
                    MAPPED_FILE >,
                    MAPPED_FILE
                    > tsync_type;

            tsync_type tsync;

            typename pe_file_controller<MAPPED_FILE>::load_ocl_system_type load_ocl_system;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;
    };

}


#endif /* FILETYPES_PE_FILE_CONTROLLER__HPP */
