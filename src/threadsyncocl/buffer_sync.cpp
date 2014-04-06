#include "buffer_sync.hpp"
#define MAX_BUFFER_SIZE 500000

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

        //this->size_buff = 0;
        this->buff = new Buffer;
        //this->buff->buffer_length = 0;

        // logger
        logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();
        //logger->write_info("BufferSync, init size of size_buff");
    }


    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSync<Buffer, MAPPED_FILE>::set_buffer(uint8_t  buffer_size)
    {

        //intial size of vector support number of thread.
        // max size all binary file.

        /*
        if(buff != NULL) {
            buff->binary_hex.resize(buffer_size);
            buff->index_binary_result.resize(buffer_size);
            return true;
        }
        		*/

        //if(this->buff->buffer_length >= MAX_BUFFER_SIZE)
        //    return false;

        //this->buff->buffer_length++;
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

        for(typename Buffer::size_int size_count =  0; size_count < size_buff; size_count++) {
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
    bool BufferSync<Buffer, MAPPED_FILE>::write_binary_hex(const char *char_hex,
            uint64_t size_hex,
            uint64_t thread_id)
    {
        //check binary length

        std::map<uint64_t,struct slot_ocl *>   *map_thread_id =
                        &buff->data_ocl_process<MAPPED_FILE>::map_tidslot_ocl;

        typename data_ocl_process<MAPPED_FILE>::map_thread_id_type::iterator iter_maptid;

        uint64_t temp_start = 0;

        struct slot_ocl *s_ocl;

        for(iter_maptid = map_thread_id->begin(); iter_maptid != map_thread_id->end(); ++iter_maptid) {
            std::pair<uint64_t, struct slot_ocl *> pair_s_ocl = *iter_maptid;
            s_ocl = pair_s_ocl.second;

            if(temp_start < s_ocl->end_point) {
                temp_start = s_ocl->end_point;
                std::cout<<" Temp_start is : "<< temp_start <<std::endl;
            }

        }

        //Temp_start for next element (start_point + 1)
        //end_point = binary size;
        //support dynamic allocator.
        buff->data_ocl_process<MAPPED_FILE>::
        binary_hex.insert(buff->data_ocl_process<MAPPED_FILE>::binary_hex.end(),
                char_hex,
                char_hex + size_hex); // insert  char hex to vector elements.

        // find thread_id and insert lenght, start_point and end_point
        iter_maptid = map_thread_id->find(thread_id);

        // insert lenght of binary hex char to s_ocl that it's contains in value of map.
        if(iter_maptid != map_thread_id->end()) {
            std::pair<uint64_t, struct slot_ocl *> pair_s_ocl = *iter_maptid;
            s_ocl  = pair_s_ocl.second;

            if(temp_start ==  0)
                s_ocl->start_point = temp_start;
            else
                s_ocl->start_point = temp_start + 1; // end point of previous + 1.

            s_ocl->end_point   = s_ocl->start_point + size_hex;
            return true;
        }

        return false;
    }

    //}


    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSync<Buffer, MAPPED_FILE>::threadbuff_regis(uint64_t thread_id)
    {
        // file_name_md5 is thread_id
        //map_thread_id_type
        std::map<uint64_t,struct slot_ocl *> *map_thread_id =
                        &buff->data_ocl_process<MAPPED_FILE>::map_tidslot_ocl;
        //map_thread_id_type

        std::pair<int, struct slot_ocl *> pair_slot;

        pair_slot.first  = thread_id;
        struct slot_ocl   *s_ocl =  pair_slot.second;
        s_ocl = new struct slot_ocl;
        s_ocl->processes_id_register = thread_id;

        if(!map_thread_id->insert(std::make_pair(thread_id, s_ocl)).second) {
            //TODO: thread_id(file_name_md5) has on map
            return false;
        }

        return true;
    }

    template class BufferSync<struct data_ocl_process<MAPPED_FILE_PE>, MAPPED_FILE_PE>;

}
