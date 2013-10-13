#include "threadsyncocl/thread_controller.hpp"
#include "threadsyncocl/buffer_sync.hpp"

using namespace controller;

TEST(thread_controller, thread_init)
{
		
		//SCOPED_TRACE("TEST THREAD_CONTROLLER");
    typedef BufferSync<buffer_kernel> buffsync;
    buffsync *buff_sync_d = new BufferSync<buffer_kernel>();
    boost::shared_ptr<comm_thread_buffer<buffsync> >  
			thread1(new comm_thread_buffer<buffsync>(1, buff_sync_d ));
		thread1->start();
		thread1->run();		

    boost::shared_ptr<comm_thread_buffer<buffsync> >  
			thread2(new comm_thread_buffer<buffsync>(1, buff_sync_d ));
		thread2->start();
		thread2->run();		

}


