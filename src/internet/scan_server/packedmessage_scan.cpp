#include "internet/scan_server/packedmessage_scan.hpp"

namespace internet
{

    template<typename MessageType>
    unsigned packedmessage_scan<MessageType>::decode_header(const data_buffer& buffer)const
    {
				LOG(INFO)<<" Decode header buffer.";
				if(buffer.size() < HEADER_SIZE){
					return 0;
				}
				unsigned msgs_size = 0;
				for(unsigned count_header = 0; count_header < HEADER_SIZE; count_header++)
				{
								msgs_size = msgs_size * 256 + (static_cast<unsigned>(buffer[count_header]) & 0xFF);
                LOG(INFO)<<"Buffer data : " << (static_cast<unsigned>(buffer[count_header]) & 0xFF);
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
    typename packedmessage_scan<MessageType>::message_pointer packedmessage_scan<MessageType>::get_msg()
    {
        return msgs;
    }

    template class packedmessage_scan<message_scan::ResponseScan>;
    template class packedmessage_scan<message_scan::RequestScan>;

}
