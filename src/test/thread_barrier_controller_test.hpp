#include "threadsyncocl/thread_barrier_controller.hpp"

TEST(Barrier_Controller, Barrier_wait)
{

		//barrier<barrier_thread> * barrier_t;
		//typename barrier_thread::buffer::thread_int thread_num = 2;
		const uint64_t  thread_num  = 2;
		barrier_controller<barrier_thread> * barrier_c = 
			new barrier_buffer<barrier_thread>(thread_num);
	  barrier<barrier_thread> * barrier_t = dynamic_cast<barrier<barrier_thread>* >(barrier_c);	
		//ASSERT_EQ(true, barrier_t->barrier_init());
		ASSERT_EQ(true, barrier_t->barrier_init(barrier_c));
		//ASSERT_EQ(true, barrier_t->barrier_wait(barrier_c));
		//barrier->barrier_wait(barrier_c);
		//barrier->barrier_condition(barrier_c);
}
