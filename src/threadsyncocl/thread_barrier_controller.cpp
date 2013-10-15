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

 			logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
			logger = logger_ptr->get();
			 int error = pthread_barrier_init(&p_barrier, NULL, thread_num_);	
			 if(!error){
					logger->write_info("barrier_buffer, init compeleted");
					return true;
				}
			 return false;
		}

		template<typename BarrierThread>
		bool barrier_buffer<BarrierThread>::condition_init()
		{
				int error = 0;
				error |=  pthread_cond_init(&p_cond, 0);
				error =   pthread_condattr_init(&p_condattr);
				if(!error){
					logger->write_info("barrier_buffer, init condition attribute");
					return true;
				}
				return false;
		}

		template<typename BarrierThread>
		bool barrier_buffer<BarrierThread>::barrier_wait()
		{
				int error = pthread_barrier_wait(&p_barrier);
				if(!error){
					return true;
					logger->write_info("barrier_buffer, wait condition");
				}
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
