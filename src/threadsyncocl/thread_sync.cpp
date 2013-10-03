#include "threadsyncocl/thread_sync.hpp"
#include "gtest/gtest.h"

namespace controller
{

template<typename Sync>
void thread_sync<Sync>::pre_check_struct()
{

std::cout<<"test"<<std::endl;
ASSERT_EQ(std::string("chatsiri").size(), std::string("rattana").size()) <<" Not enqual ";

}

template<typename Sync>
thread_sync<Sync>::thread_sync()
{

}


template<typename Sync>
boost::shared_ptr<Sync> & thread_sync<Sync>::buffer_ocl()
{
	return buffer_sync;
}

template class controller::thread_sync<int>;

}

