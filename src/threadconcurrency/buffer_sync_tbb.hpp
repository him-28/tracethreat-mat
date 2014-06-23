#ifndef CONTROLLER__BUFFER_SYNC_TBB_HPP
#define CONTROLLER__BUFFER_SYNC_TBB_HPP

//Concurrency for vector.
#include "tbb/concurrent_vector.h"

// STL
#include  <iostream>

// 3rd
#include "boost/shared_ptr.hpp"

// internal
#include "utils/logger/clutil_logger.hpp"

#include "memory/file_shm_handler.hpp"

#include "filetypes/pe.hpp"

namespace controller
{
    namespace h_util     = hnmav_util;
    namespace shm_memory = memory;


    template<typename Buffer, typename MAPPED_FILE> class  BufferSyncTBB;


    //_________________________ Slot support TBB  ___________________________


    template<typename MAPPED_FILE>
    struct data_tbb_process {
			  //Key : MD5 of file. Value : Structure such MAPPED_FILE_PE
			  //Slot replace from slot_tbb to MAPPED_FILE
        typedef std::map<uint64_t, MAPPED_FILE *>   fmd5_map_type;

        fmd5_map_type  fmd5_tbb_map;

        tbb::concurrent_vector<char> binary_hex;   // input hex of char type.

				MAPPED_FILE  * mapped_file;
    };


    //_________________________ BufferSync __________________________________
    template<typename Buffer, typename MAPPED_FILE>
    class BufferSyncTBB
    {
        public:
            typedef Buffer  buffer_internal;

            BufferSyncTBB(uint8_t  buffersync_size);
            BufferSyncTBB();
            ~BufferSyncTBB();
            //legacy concept.
            bool set_buffer(uint8_t buffer_size);

            //TODO: Plan-00003: volatile Buffer *buff;
            Buffer *buff;

            BufferSyncTBB<Buffer, MAPPED_FILE>& operator[](uint8_t value)const;
            BufferSyncTBB<Buffer, MAPPED_FILE>& operator=(BufferSyncTBB<Buffer, MAPPED_FILE> *buffr);
            BufferSyncTBB<Buffer, MAPPED_FILE>& operator*(BufferSyncTBB<Buffer, MAPPED_FILE>& buffr);

            //File MD5 register for TBB
            bool filemd5_regis_tbb(uint64_t fmd5_id, const char * file_name);


            bool write_binary_hex_tbb(const char *char_hex,
                    uint64_t size_hex,
                    uint64_t fmd5_id);


            /**
            * @brief Insert hex char to tbb_vector_concurrency. Default char_hex is byte per file.
            *
            * @param char_hex   Hex representive of char type. It's binary of file.
            * @param size_hex   Size of vector hex.
            *
            * @return True, if not problem after inserts data to tbb_vector_concurrency.
            */
            bool setbuff_tbb(const char *char_hex, uint64_t temp_start, uint64_t size_hex);


            bool set_size_summary(uint64_t size_summary);

        private:
            uint8_t size_buff;
            BufferSyncTBB<Buffer, MAPPED_FILE> *buffersync_ptr;

						MAPPED_FILE   *s_tbb_ptr;

            std::vector<char> *binary_hex_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* CONTROLLER__BUFFER_SYNC_TBB_HPP */
