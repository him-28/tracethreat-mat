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

#include "utils/base/system_code.hpp"


#include "memory/file_shm_handler.hpp"

#include "threadsyncocl/thread_sync.hpp"

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"

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

    //using namespace  controller;
    //namespace utils  = util;
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

            pe_file_controller();
            /**
            * @brief Get PE Header file from file system.
            *
            * @param mapped_file_vec  Insert file detail from memory mapped.
            *
            * @return Vector contains header file of PE.
            */
            std::vector<struct IMAGE_NT_HEADERS *>&
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
            //support small file scanning.
            /**
            * @brief Retrive data from PE file by containning on vector
            *
            * @param pe_header_vec_ptr  PE Header details from list_pe_header member functions.
            *
            * @return
            */
            uint8_t retrive_offset_lite(std::vector<MAPPED_FILE *>  pe_header_vec_ptr,
                    std::vector<struct IMAGE_NT_HEADERS * > pe_header)const;

            /**
            * @brief Get offset file per struct
            *
            * @param pe_header_ptr  contains data and size
            *
            * @return  retrun extens of struct IMAGE_NT_HEADER
            */
            struct IMAGE_NT_HEADERS_EXT& retrive_offset(MAPPED_FILE *pe_map_ptr,
                    IMAGE_NT_HEADERS *pe_header)const;

            /**
            * @brief Convert * buffer of file to vector
            *
            * @param data  buffer data.(uint8_t)
            *
            * @return convert file completed.
            */
            bool convert2buffer(uint8_t   *data, size_t size);

            /**
            * @brief Scan file with pe type
            *
            * @param file_buffer_vec   Vector type uint8_t contain all buffer file.
            *
            * @return scan completed return true.
            */

            utils::scan_file_code scan(std::vector<char> *symbol_vec,
                    std::vector<size_t> *state_vec,
                    std::vector<uint8_t> *file_buffer_vec);

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
										signature_shm_type * sig_shm);


						/*
						utils::scan_file_code scan(std::vector<MAPPED_FILE *> *mapped_file_pe_vec,
										signature_shm_type * sig_shm);
						*/

            /**
            * @brief Buffer return to external class
            *
            * @return uint64_t byte of buffer contains on std::vector<uint64_t>
            */
            std::vector<uint8_t>& get_file_buffer();


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

            std::vector<boost::shared_ptr<std::vector<struct IMAGE_NT_HEADERS *> > > pe_header_vec_shared;
            //retrive_offset_lite
            boost::shared_ptr<std::vector<struct IMAGE_NT_HEADERS_EXT *> >  pe_offset_vec_shared_ptr;
            // file buffer
            std::vector<uint8_t> file_buffer_vec;

            // add buffer and address of file.
            //std::vector<uint8_t*> file_buff_addr_vec;
            std::map<uint8_t *, size_t> file_buff_addr_map;

            //kernel file path
            std::string *kernel_file_path_ptr;


            //set file_shm
            memory::file_shm_handler<MAPPED_FILE>  f_shm_handler;

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
