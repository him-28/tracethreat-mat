#include "threadsyncocl/thread_controller.hpp"

template<typename ThreadSync>
void thread_controller<ThreadSync>::get_data()
{
		std::cout<<"Thread controller get_data " <<std::endl;
}

template<typename ThreadSync>
thread_controller<ThreadSync>::thread_controller()
{

}

template class thread_controller<int>;

