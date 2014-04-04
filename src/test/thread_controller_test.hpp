#include "threadsyncocl/thread_controller.hpp"
#include "threadsyncocl/buffer_sync.hpp"

using namespace controller;

TEST(thread_controller, thread_init)
{
		
		//SCOPED_TRACE("TEST THREAD_CONTROLLER");
    typedef BufferSync<struct data_ocl_process<MAPPED_FILE_PE>, MAPPED_FILE_PE> buffsync;

    buffsync *buff_sync_d = new BufferSync<struct data_ocl_process<MAPPED_FILE_PE>, MAPPED_FILE_PE>();

    mutex_buffer<Mutex> * mutex_internal = new mutex_buffer<Mutex>;

    boost::shared_ptr<comm_thread_buffer<buffsync, MAPPED_FILE_PE> >  
			thread1(new comm_thread_buffer<buffsync, MAPPED_FILE_PE>(1, buff_sync_d,  mutex_internal ));
		thread1->start();
		thread1->run();		

    boost::shared_ptr<comm_thread_buffer<buffsync, MAPPED_FILE_PE> >  
			thread2(new comm_thread_buffer<buffsync, MAPPED_FILE_PE>(1, buff_sync_d, mutex_internal ));
		thread2->start();
		thread2->run();		

}


