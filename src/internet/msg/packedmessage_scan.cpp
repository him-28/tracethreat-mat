#include "internet/msg/packedmessage_scan.hpp"

namespace internet
{

    template<typename MessageType>
    unsigned packedmessage_scan<MessageType>::
    decode_header(const data_buffer& buffer)const
    {
        LOG(INFO)<<" Decode header buffer.";

        if(buffer.size() < HEADER_SIZE) {
            return 0;
        }

        unsigned msgs_size = 0;

        for(unsigned count_header = 0; count_header < HEADER_SIZE; ++count_header) {
            msgs_size =
                    msgs_size * 256 +
                    (static_cast<unsigned>(buffer[count_header]) & 0xFF);

            LOG(INFO)<<"Buffer data : " <<(static_cast<unsigned>(buffer[count_header]) & 0xFF);
        }

        return msgs_size;
    }

    template<typename MessageType>
    bool packedmessage_scan<MessageType>::unpack(const data_buffer& buffer)
    {
        LOG(INFO)<<" Unpack buffer size " << buffer.size();

        return msgs->ParseFromArray(&buffer[HEADER_SIZE], buffer.size() - HEADER_SIZE);
    }

    template<typename MessageType>
    typename packedmessage_scan<MessageType>::
    message_pointer packedmessage_scan<MessageType>::get_msg()
    {
        return msgs;
    }

    template<typename MessageType>
    bool packedmessage_scan<MessageType>::pack(data_buffer& buffer)const
    {

        if(!msgs)
            return false;

        unsigned msg_size = msgs->ByteSize();
        buffer.resize(HEADER_SIZE + msg_size);
        //Included header file.
        encode_header(buffer, msg_size);

				LOG(INFO)<<" Pack message, msg_size : "<< 
					msg_size <<", buffer : "<<buffer.size();

        return msgs->SerializeToArray(&buffer[HEADER_SIZE], msg_size);

    }

    template<typename MessageType>
    void packedmessage_scan<MessageType>::
    encode_header(data_buffer& buffer, unsigned size)const
    {

            buffer[0] = static_cast<uint8_t>((size >> 24) & 0xFF);
            buffer[1] = static_cast<uint8_t>((size >> 16) & 0xFF);
            buffer[2] = static_cast<uint8_t>((size >> 8) & 0xFF);
            buffer[3] = static_cast<uint8_t>(size & 0xFF);

    }



    template class packedmessage_scan<message_scan::ResponseScan>;
    template class packedmessage_scan<message_scan::RequestScan>;

}
