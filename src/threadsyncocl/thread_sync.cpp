#include "threadsyncocl/thread_sync.hpp"

template<typename Sync>
void thread_sync<Sync>::get_data()
{
tc.get_data();   
std::cout<<"test"<<std::endl;

}

template<typename Sync>
thread_sync<Sync>::thread_sync()
{

}

template class thread_sync<int>;
//template class thread_controller<int>;

