#ifndef MEMORY_QUEUE_SHM_CONTROLLER_HPP
#define MEMORY_QUEUE_SHM_CONTROLLER_HPP

/*
* Copyright 2014 Chatsiri Rattana.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                          Authors	         Date
 * -Queue SHM controller is insert message from result of scanning. Frist initial version support TBB
 *  scanning virus.                                       R.Chatsiri       23/10/2014
 * - Fork new process when queue full.                    R.Chatsiri
 */

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>

#include "folly/io/IOBufQueue.h"
#include "folly/MPMCQueue.h"
#include "folly/detail/Futex.h"

#include <memory>

#include "msg/message_tracethreat.pb.h"



namespace memory
{
    namespace boostinp = boost::interprocess;

		/*
		template<typename Relocate>
		struct buffer{
			typedef Relocate IsRelocatable;
			bool constructed;

			folly::IOBuf * io_buf;

			buffer() noexcept : constructed(true) {
					io_buf  = new folly::IOBuf();
			}

		  buffer(buffer & _buffer) : constructed(true) {
					
			}


		  buffer(folly::IOBuf & io_buff) : constructed(true) {
					this->io_buf = io_buf;
			}

			~buffer() noexcept {
					delete this->io_buf;
			}

		};
		*/

    template<typename MessageType = scan_threat::InfectedFileInfo>
    class queue_shm_controller
    {

        public:

            typedef folly::IOBuf io_buf;
            //IOBuf allocator
            typedef boostinp::allocator<io_buf, boostinp::managed_shared_memory::segment_manager>
            io_buf_allocator;

            typedef std::pair<std::string, io_buf_allocator::pointer> value_type_io_buf;

            typedef std::string key_types;

            typedef boostinp::allocator<value_type_io_buf,
                    boostinp::managed_shared_memory::segment_manager> map_io_buf_allocator;

            typedef std::map<key_types,
                    value_type_io_buf,
                    std::less<key_types>,
                    map_io_buf_allocator> map_io_buf;

            typedef boost::shared_ptr<MessageType> msgs_type_ptr;

            queue_shm_controller(uint64_t shm_size);

            /**
            * @brief Changed message type contains in buffer.
            *
            * @param msg_type Message type such as : InfectedResult.
            *
            * @return  Pointer of io_buffer
            */
            std::unique_ptr<folly::IOBuf> message_buff(MessageType& msg_type);

            /**
            * @brief Append IO-Buffer to Queue of IO-Buffer
            *
            * @param iobuf_msg_type  Mesage type contain in buffer.
            *
            * @return
            */
            bool append(std::unique_ptr<folly::IOBuf> iobuf_msg_type);

            /**
            * @brief End steps write to memory. All message write to memory.
            *
            * @param iobuf_queue  IO-Buffer-Queue created contains all message(IO-Buff)
            * after scan per request.
            *
            * @return False, If system cannot write infected message results to memroy.
            */
            bool write_msg(folly::IOBufQueue& iobuf_queue);


            /**
            * @brief Add IO-Buffer Queue and send to Storage directly.
            *
            * @param iobuf_queue  IO-Buffer Queue.
            *
            * @return False, If API of database return false.
            */
            bool write_storage(folly::IOBufQueue& iobuf_queue);

        private:

            folly::IOBufQueue *message_queue;
						//folly::MPMCQueue<buffer<std::true_type>> * mpmcq;

    };

}


#endif /* MEMORY_QUEUE_SHM_CONTROLLER_HPP */
