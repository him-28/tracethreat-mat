#include "tbb/parallel_for.h"

#include "threadconcurrency/buffer_sync_tbb.hpp"
#include <stdexcept>
#include <algorithm>
#include <iterator>


namespace controller
{

    template<typename Buffer, typename MAPPED_FILE>
    BufferSyncTBB<Buffer, MAPPED_FILE>::BufferSyncTBB(uint8_t  buffersync_size)
    {

        size_buff = buffersync_size > 0 ? buffersync_size : 1;
        buffersync_ptr = new BufferSyncTBB<Buffer,MAPPED_FILE>[size_buff];

        for(int count_buff = 0; count_buff < size_buff; count_buff++) {
            buffersync_ptr[count_buff] = new BufferSyncTBB<Buffer, MAPPED_FILE>();
        }

    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSyncTBB<Buffer, MAPPED_FILE>::BufferSyncTBB()
    {

        this->buff = new Buffer;

    }


    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSyncTBB<Buffer, MAPPED_FILE>::set_buffer(uint8_t  buffer_size)
    {
        //TODO:
        return true;
    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSyncTBB<Buffer, MAPPED_FILE>& BufferSyncTBB<Buffer, MAPPED_FILE>::
    operator[](uint8_t value)const
    {

        if(value < 0 || value >= size_buff)
            logger->write_info("BufferSyncTBB, Size error ");// todo : error try catch

        return buffersync_ptr[value];

    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSyncTBB<Buffer, MAPPED_FILE>& BufferSyncTBB<Buffer, MAPPED_FILE>::
    operator=(BufferSyncTBB<Buffer, MAPPED_FILE> *buffr)
    {


        if(buffr != this) {
            if(size_buff != buffr->size_buff) {
                delete [] buffersync_ptr;
                size_buff = buffr->size_buff;
                buffersync_ptr = new BufferSyncTBB[size_buff];
            }
        }

        for(uint8_t size_count =  0; size_count < size_buff; size_count++) {
            buffersync_ptr[size_count] = buffr[size_count];
        }

        return *this;

    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSyncTBB<Buffer, MAPPED_FILE>& BufferSyncTBB<Buffer, MAPPED_FILE>::
    operator*(BufferSyncTBB<Buffer, MAPPED_FILE>& buffr)
    {
        //todo:
    }


    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSyncTBB<Buffer, MAPPED_FILE>::filemd5_regis_tbb(uint64_t fmd5_id,
            const char *file_name)
    {

        // file_name_md5 is thread_id
        std::map<uint64_t, MAPPED_FILE *> *map_fmd5_id =
                &buff->data_tbb_process<MAPPED_FILE>::fmd5_tbb_map;

        std::pair<int,MAPPED_FILE *> pair_slot;

        pair_slot.first  = fmd5_id;

        s_tbb_ptr =  pair_slot.second;
        s_tbb_ptr = new MAPPED_FILE;
        s_tbb_ptr->file_map_md5 = fmd5_id;
        s_tbb_ptr->file_name    = file_name;
        s_tbb_ptr->end_point    = 0;

        if(!map_fmd5_id->insert(std::make_pair(fmd5_id, s_tbb_ptr)).second) {
            //TODO: thread_id(file_name_md5) has on map
            return true;
        }

        return false;


    }


    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSyncTBB<Buffer, MAPPED_FILE>::write_binary_hex_tbb(const char *char_hex,
            uint64_t size_hex,
            uint64_t fmd5_id)
    {
        //check binary length
        logger->write_info("BufferSyncTBB::write_binary_hex_tbb, file size",
                boost::lexical_cast<std::string>(size_hex));

        std::map<uint64_t, MAPPED_FILE *>   *map_fmd5_id =
                &buff->data_tbb_process<MAPPED_FILE>::fmd5_tbb_map;

        typename data_tbb_process<MAPPED_FILE>::fmd5_map_type::iterator iter_mapfmd5;

        uint64_t temp_start = 0;

        MAPPED_FILE *s_tbb;

        for(iter_mapfmd5 = map_fmd5_id->begin(); iter_mapfmd5 != map_fmd5_id->end(); ++iter_mapfmd5) {
            std::pair<uint64_t, MAPPED_FILE *> pair_s_tbb = *iter_mapfmd5;
            s_tbb = pair_s_tbb.second;

            if(temp_start <= s_tbb->end_point) {
                temp_start = s_tbb->end_point;

                logger->write_info("BufferSyncTBB::write_binary_hex_tbb(), Temp start point ",
                        boost::lexical_cast<std::string>(temp_start));
            }

        }

        //Temp_start for next element (start_point + 1)
        //end_point = binary size;
        //support dynamic allocator.
        //start_point = Temp_start;
        //end_point   = temp_start + size_hex;
        if(setbuff_tbb(char_hex,temp_start, size_hex)) {
            //TODO: If no data insert to binary_hex
        }


        // find thread_id and insert lenght, start_point and end_point
        iter_mapfmd5 = map_fmd5_id->find(fmd5_id);

        // insert lenght of binary hex char to s_ocl that it's contains in value of map.
        if(iter_mapfmd5 != map_fmd5_id->end()) {
            std::pair<uint64_t, MAPPED_FILE *> pair_s_tbb = *iter_mapfmd5;
            s_tbb  = pair_s_tbb.second;

            s_tbb->start_point = temp_start;

            s_tbb->end_point   = s_tbb->start_point + size_hex;
            std::cout<<"Start point " << temp_start <<", End point : " << s_tbb->end_point <<std::endl;
            std::cout<<"Map map_fmd5_id size(fmd5_tbb_map) : " << map_fmd5_id->size() <<std::endl;
            return true;
        }

        return false;
    }


    //Append parallel insert binary stream to concurren_vector
    struct AppendBuffer {
        typedef tbb::concurrent_vector<char> binary_hex_type;
        binary_hex_type& binary_hex_;
        char *str_hex_;
        AppendBuffer(binary_hex_type& _binary_hex, char *str_hex) :
            binary_hex_(_binary_hex),
            str_hex_(str_hex) { }
        void operator()(const tbb::blocked_range<uint64_t>&   range) const {

            for(uint64_t count_strhex = range.begin();
                    count_strhex != range.end();
                    ++count_strhex) {
                binary_hex_.push_back(str_hex_[count_strhex]);
            }// for
        }// operator ()

    };


    //Setbuff_tbb sets buffer to tbb::concurrent_vector<char> binary_hex;
    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSyncTBB<Buffer, MAPPED_FILE>::setbuff_tbb(const char *char_hex,
            uint64_t temp_start,
            uint64_t size_hex)
    {

        if(size_hex == 0)
            return false;


        //http://www.devx.com/cplus/Article/33334/0/page/2
        //memcpy(&binary_x.at(*binary_x.grow_by(size_hex)), char_hex , size_hex + 1);
        //memcpy(&buff->binary_hex[*buff->binary_hex.grow_by(size_hex)], char_hex, size_hex+1);

        //	tbb::parallel_for(tbb::blocked_range<uint64_t>(0, size_hex), append_buff());
        buff->binary_hex.insert(buff->binary_hex.end(),
                char_hex,
                char_hex + size_hex); // insert char hex to vector elements.


        //https://gist.github.com/Chatsiri/3756912b63b1f325c788
        //tbb::parallel_for(tbb::blocked_range<uint64_t>(0, size_hex),
        //	AppendBuffer(buff->binary_hex, const_cast<char*>(char_hex)));

        return true;
    }


    template<typename Buffer, typename MAPPED_FILE>
    bool BufferSyncTBB<Buffer, MAPPED_FILE>::set_size_summary(uint64_t size_summary)
    {
        size_summary = size_summary * 2 + 1;

        //buff->binary_hex = tbb::concurrent_vector<char>(size_summary);
        buff->binary_hex = std::vector<char>(size_summary);

        if(buff->binary_hex.size() != size_summary)
            return false;

        return true;
    }

    template<typename Buffer, typename MAPPED_FILE>
    BufferSyncTBB<Buffer, MAPPED_FILE>::~BufferSyncTBB()
    {

        delete buff;

    }

    template class BufferSyncTBB<struct data_tbb_process<MAPPED_FILE_PE>, MAPPED_FILE_PE>;

}
