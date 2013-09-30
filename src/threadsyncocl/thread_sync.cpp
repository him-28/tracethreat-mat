#include "threadsyncocl/thread_sync.hpp"
#include "gtest/gtest.h"

namespace controller
{

template<typename Sync>
void thread_sync<Sync>::get_data()
{
tc.get_data();   
std::cout<<"test"<<std::endl;
ASSERT_EQ(std::string("chatsiri").size(), std::string("rattana").size()) <<" Not enqual ";
}

template<typename Sync>
thread_sync<Sync>::thread_sync()
{

}

}

template class controller::thread_sync<int>;
