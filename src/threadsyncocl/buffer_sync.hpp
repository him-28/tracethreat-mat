#ifndef CONTROLLER__BUFFER_SYNC_HPP_
#define CONTROLLER__BUFFER_SYNC_HPP_
// STL
#include  <iostream>

// 3rd
#include "CL/cl.h"

// internal
#include "utils/logger/clutil_logger.hpp"


#define MAX_LENGTH 1024

namespace controller
{
    namespace h_util = hnmav_util;
    //Declare forward which caller class.
    struct buffer_kernel;
    template<typename Buffer> class  BufferSync;

    struct slot_ocl {
        uint8_t  status;
        uint64_t start_point; //start point of binary file.
        uint64_t end_point;   //end point of binary file.
        uint64_t processes_id_register; //insert processes thread_id;
			  uint64_t file_map_md5; // check md5 match with file-shm mapping index.
    };

    template<typename MAPPED_FILE>

    struct data_ocl_process {
        typedef file_shm_handler<MAPPED_FILE>::map_str_shm mapstr_shm_type;

        mapstr_shm_type *mapstr_shm;   // shared_memory system.
        std::vector<char> binary_hex;  // input hex of char type.
        std::vector<int>  index_binary_result; // index of vector binaries are hex of char type.
        std::map<processes_id_register,struct slot_ocl *> slot_ocl_rank;
    };

    struct buffer_kernel {
        cl_int  buffer_length;
        cl_char buffer_send;
        typedef int thread_int;
        typedef int size_int;
        //Support buffer block status
    };

    template<typename Buffer>
    class BufferSync
    {
        public:
            BufferSync(typename buffer_kernel::size_int  buffersync_size);
            BufferSync();

						//legacy concept.
            bool set_buffer(controller::buffer_kernel::size_int, char *filename);

            volatile Buffer *buff;

            BufferSync<Buffer>& operator[](typename Buffer::size_int value)const;
            BufferSync<Buffer>& operator=(BufferSync<Buffer> *buffr);
            BufferSync<Buffer>& operator*(BufferSync<Buffer>& buffr);

						//thread register
						bool thread_buff_regis(uint64_t thread_id);
						bool threadbuff_sync_slotocl(uint64_t thread_id);
						std::vector<int> threadbuff_result()const;
        private:
            typename Buffer::size_int size_buff;
            BufferSync *buffersync_ptr;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
