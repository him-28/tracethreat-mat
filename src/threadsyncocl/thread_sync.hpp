#ifndef THREAD_SYNC_HPP
#define THREAD_SYNC_HPP

#include <iostream>
#include "threadsyncocl/thread_controller.hpp"
#include "ocl/cl_bootstrap.hpp"

namespace controller
{

template<typename BufferSync>
class ibuffer_sync
{
	public:
		virtual boost::shared_ptr<BufferSync> buffer_ocl() = 0;

};

template<typename Sync>
class thread_sync : public ibuffer_sync<Sync>
{
public:

thread_sync();

boost::shared_ptr<Sync> buffer_ocl();


// for test case only
void pre_check_struct();

private:
thread_controller<int> tc;
boost::shared_ptr<Sync> buffer_sync;
};

}

#endif /* THREAD_THREAD_SYNC_HPP */
