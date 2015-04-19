#include "memory/queue_shm_controller.hpp"

namespace memory
{
		//message_tracethreat::InfectedFileInfo

    template<typename MessageType>
    queue_shm_controller<MessageType>::queue_shm_controller(uint64_t shm_size)
  	{
        folly::IOBufQueue::Options options;
        options.cacheChainLength = true;
        message_queue = new folly::IOBufQueue(options);
    }

    template<typename MessageType>
    std::unique_ptr<folly::IOBuf> queue_shm_controller<MessageType>::
    message_buff(MessageType& msg_type)
    {
        std::unique_ptr<folly::IOBuf> buf = folly::IOBuf::create(msg_type.ByteSize());
        memcpy(buf->writableTail(), &msg_type, msg_type.ByteSize());
        buf->append(msg_type.ByteSize());
        return std::move(buf);
    }

    template<typename MessageType>
    bool queue_shm_controller<MessageType>::append(std::unique_ptr<folly::IOBuf> iobuf_msg_type)
    {

        message_queue->append(&iobuf_msg_type, iobuf_msg_type->countChainElements());
        if(message_queue->empty())
            return false;

        return true;
    }

		/* Write before read */
    template<typename MessageType>
    bool queue_shm_controller<MessageType>::write_msg(folly::IOBufQueue& iobuf_queue)
    {
				
    }


    template class queue_shm_controller<scan_threat::InfectedFileInfo>;
}
