#include "utils/base/timer_queue.hpp"
#include <iostream>
//#include "gtest/gtest.h"

TEST(TimerQueue, IntervalTimes)
{
	 using utils::timer_queue;
   utils::timer_queue time_q;
	 utils::timer_queue::Id id = 1;
		
	 //nana sec
	 utils::timer_queue::Id id_check = time_q.add(id, time_q.get_start_timer());
	long int interval_t = time_q.interval_time(time_q.get_start_timer(id_check));
	 std::cout<<"Interval time : "<< interval_t << " ns " <<std::endl;

	 EXPECT_EQ(id, id_check);

	 	 // linux : micro sec
	 utils::timer_queue::Id id_check_linux = time_q.add(id, time_q.unix_time());
	 EXPECT_EQ(id, id_check_linux);
	

	 //basic milisec
   utils::timer_queue::Id id_check_basic = time_q.add(id, time_q.basic_time_sec(),
																									time_q.basic_time_usec());

	 utils::time_result time_r =  time_q.interval_time(time_q.get_stimer_basic_sec(id_check_basic), 
												time_q.get_stimer_basic_usec(id_check_basic)); 

	 printf("Time: %u.%3.3u sec (%u m %u s) \n",  time_r.ds, time_r.dms/1000, time_r.ds/60, time_r.ds%60);

		double interval_t_linux = time_q.interval_time(time_q.unix_time(), 
				time_q.get_start_timer_linux(id_check_linux));
	
	 std::cout<<"Interval time : "<< interval_t_linux << " ms " <<std::endl;
	

}
