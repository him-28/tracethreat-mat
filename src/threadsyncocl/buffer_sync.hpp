#ifndef CONTROLLER__BUFFER_SYNC_HPP_
#define CONTROLLER__BUFFER_SYNC_HPP_
// STL
#include  <iostream>

// 3rd
#include "CL/cl.h"

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
    template<typename MAPPED_FILE> struct data_ocl_process;
    template<typename Buffer, typename MAPPED_FILE> class  BufferSync;


    struct slot_ocl {
        uint8_t  status;
        uint64_t start_point; //start point of binary file.
        uint64_t end_point;   //end point of binary file.
        uint64_t processes_id_register; //insert processes thread_id;
        uint64_t file_map_md5; // check md5 match with file-shm mapping index.
    };

    template<typename MAPPED_FILE>
    struct data_ocl_process {
        typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm * mapstr_shm;
        typedef std::map<uint64_t,struct slot_ocl *>   map_thread_id_type;

        //mapstr_shm_type *mapstr_shm;   // shared_memory system.
        std::vector<char> binary_hex;  // input hex of char type.
        std::vector<int>  index_binary_result; // index of vector binaries are hex of char type.
        map_thread_id_type map_tidslot_ocl;

        //migrate from : struct name buffer_kernel.
        uint8_t buffer_length;

        typedef uint64_t thread_int;
        typedef uint8_t  size_int;

    };


    /*
    struct buffer_kernel {
    cl_int  buffer_length;
    cl_char buffer_send;
    typedef int thread_int;
    typedef int size_int;
    //Support buffer block status
    };
    */

    template<typename Buffer, typename MAPPED_FILE>
    class BufferSync
    {
        public:
            BufferSync(uint8_t  buffersync_size);
            BufferSync();

            //legacy concept.
            bool set_buffer(uint8_t buffer_size);

            //volatile Buffer *buff;
						Buffer * buff;

            BufferSync<Buffer, MAPPED_FILE>& operator[](uint8_t value)const;
            BufferSync<Buffer, MAPPED_FILE>& operator=(BufferSync<Buffer, MAPPED_FILE> *buffr);
            BufferSync<Buffer, MAPPED_FILE>& operator*(BufferSync<Buffer, MAPPED_FILE>& buffr);

            //thread register
            bool threadbuff_regis(uint64_t thread_id);

            bool threadbuff_sync_slotocl(uint64_t thread_id);

            std::vector<int> threadbuff_result()const;

            bool write_binary_hex(const char *char_hex, uint64_t size_hex, uint64_t thread_id);

        private:
            typename Buffer::size_int size_buff;
            BufferSync<Buffer, MAPPED_FILE> *buffersync_ptr;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
