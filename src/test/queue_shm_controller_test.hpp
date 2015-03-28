
#include "folly/io/IOBufQueue.h"

#include "memory/queue_shm_controller.hpp"

#include "msg/message_tracethreat.pb.h" 

TEST(Queue_shm_controller, initial_value)
{
	typedef boost::shared_ptr<message_tracethreat::InfectedFileInfo> infected_info_ptr;
	infected_info_ptr  infected_info(new  message_tracethreat::InfectedFileInfo);
	memory::queue_shm_controller<message_tracethreat::InfectedFileInfo> queue_shm(1024);
	EXPECT_TRUE(queue_shm.append(queue_shm.message_buff(*infected_info.get())));

};
