#ifndef CONTROLLER__BUFFER_SYNC_HPP
#define CONTROLLER__BUFFER_SYNC_HPP

//Concurrency for vector.
#include "tbb/concurrent_vector.h"

// STL
#include  <iostream>

// 3rd
#include "CL/cl.h"
#include "boost/shared_ptr.hpp"

// internal
#include "utils/logger/clutil_logger.hpp"

#include "memory/file_shm_handler.hpp"

#include "filetypes/pe_template.hpp"
//#define MAX_LENGTH 1024

namespace controller
{
    namespace shm_memory = memory;

    //Declare forward which caller class.
    //struct buffer_kernel;

    struct slot_ocl;
    struct data_sig_process;

    template<typename Buffer, typename MAPPED_FILE> class  BufferSync;

    //_________________________ Slot support OCL ___________________________
    struct slot_ocl {
        uint8_t  status;
        uint64_t start_point; //start point of binary file.
        uint64_t end_point;   //end point of binary file.
        uint64_t file_map_md5; // check md5 match with file-shm mapping index.
				slot_ocl(): status(0), start_point(0), end_point(0), file_map_md5(0){ }
    };

    /**
    * @brief Plan-0003 : Support single signature scanning.
    */
    struct data_sig_process {
        const char *sig_name;  //signature name
        uint8_t      sig_type; //signature type
        uint64_t     start_index_scan; // index start scanning.
    };

    template<typename MAPPED_FILE>
    struct data_ocl_process {
        typedef std::map<uint64_t,struct slot_ocl *>   map_md5_type;

        std::vector<char> binary_hex;   // input hex of char type.
        std::vector<uint8_t> index_binary_result;  // index of vector binaries are hex of char type.
        std::vector<char> symbol_hex;

        map_md5_type map_fmd5_slot_ocl;

        //migrate from : struct name buffer_kernel.
        uint8_t buffer_length;
    };


    //_________________________ BufferSync __________________________________
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

            //TODO: Plan-00003: volatile Buffer *buff;
            Buffer *buff;

            //TODO: Plan-00003: Multiple name search virus.
            data_sig_process *sig_processes;


            BufferSync<Buffer, MAPPED_FILE>& operator[](uint8_t value)const;
            BufferSync<Buffer, MAPPED_FILE>& operator=(BufferSync<Buffer, MAPPED_FILE> *buffr);
            BufferSync<Buffer, MAPPED_FILE>& operator*(BufferSync<Buffer, MAPPED_FILE>& buffr);

      
            //File MD5 register for OCL
            bool filemd5_regis_ocl(uint64_t fmd5_id);


            bool write_binary_hex_ocl(const char *char_hex,
                    uint64_t size_hex,
                    uint64_t fmd5_id);

       
            /**
            * @brief Insert hex char to vector. Default char_hex is byte per file.
            *
            * @param char_hex  Hex representive of char type. It's binary of file.
            * @param size_hex  size of vector hex.
            *
            * @return True, If not problem after inserts data to vector<char>.
            */
            bool setbuff_ocl(const char *char_hex, uint64_t size_hex);
 

            bool set_size_summary(uint64_t size_summary);

        private:
            uint8_t size_buff;
            BufferSync<Buffer, MAPPED_FILE> *buffersync_ptr;

            std::vector<char> *binary_hex_ptr;
            std::vector<char> *symbol_hex_ptr;
            //logger
            boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
            utils::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* CONTROLLER__BUFFER_SYNC_HPP_ */
