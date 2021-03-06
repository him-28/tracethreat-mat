#include "buffer_sync.hpp"
//#define MAX_BUFFER_SIZE 500000
#include <stdexcept>
#include <algorithm>
#include <iterator>


namespace controller
{

    template<typename Buffer, typename MAPPED_FILE>
    BufferSync<Buffer, MAPPED_FILE>::BufferSync(uint8_t  buffersync_size)
    {

        size_buff = buffersync_size > 0 ? buffersync_size : 1;
        buffersync_ptr = new BufferSync<Buffer,MAPPED_FILE>[size_buff];

        for(int count_buff = 0; count_buff < size_buff; count_buff++) {
            buffersync_ptr[count_buff] = new BufferSync<Buffer, MAPPED_FILE>();
        }

    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSync<Buffer, MAPPED_FILE>::BufferSync()
    {

        this->buff = new Buffer;

    }


    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSync<Buffer, MAPPED_FILE>::set_buffer(uint8_t  buffer_size)
    {
        //TODO:
        return true;
    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSync<Buffer, MAPPED_FILE>& BufferSync<Buffer, MAPPED_FILE>::
    operator[](uint8_t value)const
    {

        if(value < 0 || value >= size_buff)
            logger->write_info("BufferSync, Size error ");// todo : error try catch

        return buffersync_ptr[value];

    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSync<Buffer, MAPPED_FILE>& BufferSync<Buffer, MAPPED_FILE>::
    operator=(BufferSync<Buffer, MAPPED_FILE> *buffr)
    {


        if(buffr != this) {
            if(size_buff != buffr->size_buff) {
                delete [] buffersync_ptr;
                size_buff = buffr->size_buff;
                buffersync_ptr = new BufferSync[size_buff];
            }
        }

        for(uint8_t size_count =  0; size_count < size_buff; size_count++) {
            buffersync_ptr[size_count] = buffr[size_count];
        }

        return *this;

    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSync<Buffer, MAPPED_FILE>& BufferSync<Buffer, MAPPED_FILE>::
    operator*(BufferSync<Buffer, MAPPED_FILE>& buffr)
    {
        //todo:
    }

    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSync<Buffer, MAPPED_FILE>::write_binary_hex_ocl(const char *char_hex,
            uint64_t size_hex,
            uint64_t fmd5_id)
    {
        //check binary length

        std::map<uint64_t,struct slot_ocl *>   *map_fmd5_id =
                        &buff->data_ocl_process<MAPPED_FILE>::map_fmd5_slot_ocl;

        typename data_ocl_process<MAPPED_FILE>::map_md5_type::iterator iter_maptid;

        uint64_t temp_start = 0;

        struct slot_ocl *s_ocl;

        for(iter_maptid = map_fmd5_id->begin(); iter_maptid != map_fmd5_id->end(); ++iter_maptid) {
            std::pair<uint64_t, struct slot_ocl *> pair_s_ocl = *iter_maptid;
            s_ocl = pair_s_ocl.second;

            if(temp_start < s_ocl->end_point) {
                temp_start = s_ocl->end_point;

                logger->write_info("BufferSync::write_binary_hex_ocl(), Temp start point ",
                        boost::lexical_cast<std::string>(temp_start));
            }

        }

        //Temp_start for next element (start_point + 1)
        //end_point = binary size;
        //support dynamic allocator.
        if(setbuff_ocl(char_hex, size_hex)) {
            //TODO: If no data insert to binary_hex
        }


        // find thread_id and insert lenght, start_point and end_point
        iter_maptid = map_fmd5_id->find(fmd5_id);

        // insert lenght of binary hex char to s_ocl that it's contains in value of map.
        if(iter_maptid != map_fmd5_id->end()) {
            std::pair<uint64_t, struct slot_ocl *> pair_s_ocl = *iter_maptid;
            s_ocl  = pair_s_ocl.second;

            s_ocl->start_point = temp_start;

            s_ocl->end_point   = s_ocl->start_point + size_hex;

            return true;
        }

        return false;
    }




    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSync<Buffer, MAPPED_FILE>::filemd5_regis_ocl(uint64_t fmd5_id)
    {
        // file_name_md5 is thread_id
        std::map<uint64_t,struct slot_ocl *> *map_fmd5_id =
                        &buff->data_ocl_process<MAPPED_FILE>::map_fmd5_slot_ocl;

        std::pair<int, struct slot_ocl *> pair_slot;

        pair_slot.first  = fmd5_id;
        struct slot_ocl   *s_ocl =  pair_slot.second;
        s_ocl = new struct slot_ocl;
        s_ocl->file_map_md5          = fmd5_id;

        if(!map_fmd5_id->insert(std::make_pair(fmd5_id, s_ocl)).second) {
            //TODO: thread_id(file_name_md5) has on map
            return false;
        }

        return true;
    }
	
    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSync<Buffer, MAPPED_FILE>::setbuff_ocl(const char *char_hex,
            uint64_t size_hex)
    {

        if(size_hex == 0)
            return false;

        buff->binary_hex.insert(buff->binary_hex.end(),
                char_hex,
                char_hex + size_hex); // insert char hex to vector elements.

        buff->index_binary_result.resize(buff->binary_hex.size());

        return true;
    }




    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSync<Buffer, MAPPED_FILE>::set_size_summary(uint64_t size_summary)
    {
        size_summary = size_summary * 2 + 1;

        buff->binary_hex = std::vector<char>(size_summary);
        buff->index_binary_result = std::vector<uint8_t>(size_summary);

        if(buff->binary_hex.size() != size_summary) return false;

        if(buff->index_binary_result.size() != size_summary) return false;
    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSync<Buffer, MAPPED_FILE>::~BufferSync()
    {

        delete buff;

    }



    template class BufferSync<struct data_ocl_process<MAPPED_FILE_PE>, MAPPED_FILE_PE>;

}
