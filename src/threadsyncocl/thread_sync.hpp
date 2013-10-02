#ifndef THREAD_SYNC_HPP
#define THREAD_SYNC_HPP

#include <iostream>
#include "threadsyncocl/thread_controller.hpp"

namespace controller
{

template<typename Sync>
class thread_sync
{
public:

thread_sync();
void pre_check_struct();
private:
thread_controller<int> tc;
};

}

#endif /* THREAD_THREAD_SYNC_HPP */
