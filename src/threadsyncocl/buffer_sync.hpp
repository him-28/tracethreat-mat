#ifndef CONTROLLER__BUFFER_SYNC_HPP_
#define CONTROLLER__BUFFER_SYNC_HPP_
// STL
#include  <iostream>

// 3rd
#include "CL/cl.h"
#include "boost/shared_ptr.hpp"

// internal
#include "utils/logger/clutil_logger.hpp"

#include "memory/file_shm_handler.hpp"

#include "filetypes/pe.hpp"
//#define MAX_LENGTH 1024

namespace controller
{
    namespace h_util     = hnmav_util;
    namespace shm_memory = memory;

    //Declare forward which caller class.
    //struct buffer_kernel;

    struct slot_ocl;
    struct data_sig_process;
    //template<typename MAPPED_FILE> struct data_ocl_process::binary_hex;
    template<typename Buffer, typename MAPPED_FILE> class  BufferSync;


    struct slot_ocl {
        uint8_t  status;
        uint64_t start_point; //start point of binary file.
        uint64_t end_point;   //end point of binary file.
        uint64_t processes_id_register; //insert processes thread_id;
        uint64_t file_map_md5; // check md5 match with file-shm mapping index.
    };

    /**
    * @brief Plan-0003 : Support single signature scanning.
    */
    struct data_sig_process {
        const char *sig_name;  //signature name
        uint8_t      sig_type; //signature type
        uint64_t     start_index_scan; // index start scanning.
        //std::vector<uint8_t> symbol_vec; //TODO: Plan-00004 : Support arena size.
        //std::vector<size_t>  state_vec;  //TODO: Plan-00004 : Support arena size.
    };

    template<typename MAPPED_FILE>
    struct data_ocl_process {
        //typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm *mapstr_shm;
        typedef std::map<uint64_t,struct slot_ocl *>   map_thread_id_type;

        //std::vector<char> *binary_hex;   // input hex of char type.
        //std::vector<uint8_t> *index_binary_result;  // index of vector binaries are hex of char type.
        std::vector<char> binary_hex;   // input hex of char type.
        std::vector<uint8_t> index_binary_result;  // index of vector binaries are hex of char type.
				std::vector<char> symbol_hex;

        map_thread_id_type map_tidslot_ocl;

        //migrate from : struct name buffer_kernel.
        uint8_t buffer_length;

        typedef uint64_t thread_int;
        typedef uint8_t  size_int;

    };


    template<typename Buffer, typename MAPPED_FILE>
    class BufferSync
    {
        public:
            typedef boost::shared_ptr<std::vector<char> > binary_hex_sptr_type;
						typedef Buffer  buffer_internal;

            BufferSync(uint8_t  buffersync_size);
            BufferSync();
					  ~BufferSync();
            //legacy concept.
            bool set_buffer(uint8_t buffer_size);

            //TODO: Plan-00004: volatile Buffer *buff;
            Buffer *buff;
            //TODO: Plan-00004: Multiple name search virus.
            data_sig_process *sig_processes;

            //struct data_ocl_process<MAPPED_FILE>  docl_process;

            BufferSync<Buffer, MAPPED_FILE>& operator[](uint8_t value)const;
            BufferSync<Buffer, MAPPED_FILE>& operator=(BufferSync<Buffer, MAPPED_FILE> *buffr);
            BufferSync<Buffer, MAPPED_FILE>& operator*(BufferSync<Buffer, MAPPED_FILE>& buffr);

            //thread register
            bool threadbuff_regis(uint64_t thread_id);

            bool threadbuff_sync_slotocl(uint64_t thread_id);

            std::vector<int> threadbuff_result()const;

            bool write_binary_hex(const char *char_hex,
                    uint64_t size_hex,
                    uint64_t thread_id);
                    //boost::shared_ptr<std::vector<char> >  binary_hex_ptr);

            /**
            * @brief Insert hex char to vector. Default char_hex is byte per file.
            *
            * @param char_hex  Hex representive of char type. It's binary of file.
            * @param binary_hex  Vector contains char hex send to OCL.
            * @param s_ocl Slot_ocl contains detail status and length of file.
            *
            * @return True, If not problem after inserts data to vector<char>.
            */
            bool setbuff_ocl(const char *char_hex,
                    uint64_t size_hex);
                    //boost::shared_ptr<std::vector<char> >  binary_hex_vec_sptr);

            bool set_size_summary(uint64_t size_summary);

        private:
            typename Buffer::size_int size_buff;
            BufferSync<Buffer, MAPPED_FILE> *buffersync_ptr;

            std::vector<char> *binary_hex_ptr;
						std::vector<char> *symbol_hex_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
