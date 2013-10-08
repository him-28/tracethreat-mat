/*                       Titles                                          Authors                        Date
 * - Barrier controller before send to ocl															 R.Chatsiri
 */

#include "threadsyncocl/thread_barrier_controller.hpp"

namespace controller
{

//    template<typename BarrierThread>
//		barrier_buffer<BarrierThread>::barrier_buffer(){ }
		
		template<typename BarrierThread>
		bool barrier_buffer<BarrierThread>::barrier_init()
		{ 
			 int error = pthread_barrier_init(&p_barrier, NULL, thread_num_);	
			 if(!error)
					return true;
			 return false;
		}

		template<typename BarrierThread>
		bool barrier_buffer<BarrierThread>::condition_init()
		{
				int error = 0;
				error |=  pthread_cond_init(&p_cond, 0);
				error =   pthread_condattr_init(&p_condattr);
				if(!error)
					return true;
				return false;
		}

		template<typename BarrierThread>
		bool barrier_buffer<BarrierThread>::barrier_wait()
		{
				int error = pthread_barrier_wait(&p_barrier);
				if(!error)
					return true;
				return false;
		} 

		template<typename BarrierThread>
		bool barrier_buffer<BarrierThread>::barrier_condition()
		{

		}


		template<typename BarrierThread>
	  bool barrier_buffer<BarrierThread>::destroy()
		{
			pthread_cond_destroy(&p_cond);
			pthread_condattr_destroy(&p_condattr);
			
		}

}
