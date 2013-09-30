#ifndef CONTROLLER__SEMAPHORE_CONTROLLER_HPP_
#define CONTROLLER__SEMAPHORE_CONTROLLER_HPP_ 

#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

namespace controller
{

template<typename SemaphoreProcesses>
class semaphore
{
	public:
		virtual lock_request()   = 0;
		virtual unlock_request() = 0;
		virtual boost::unique_ptr<SemaphoreProcesses> processes() = 0;
};

// Mutex 
template<typename Mutex>
class mutex_buffer
{
    public:
				init();
				lock_request();
				try_lock();
				unlock_request();
				destruction();
    private:
				pthread_mutex_t mutex;
};

}

#endif /* CONTROLLER__SEMAPHORE_CONTROLLER_HPP_ */
