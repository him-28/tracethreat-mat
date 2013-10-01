#ifndef CONTROLLER__SEMAPHORE_CONTROLLER_HPP_
#define CONTROLLER__SEMAPHORE_CONTROLLER_HPP_

#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

namespace controller
{

		//Define type class
		class Mutex
		{
			public: 
			   pthread_mutex_t mutex_t;
		}


		// Bule print of mutex.
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
    class mutex_buffer : public semaphore<Mutex>
    {
        public:
            bool init();
            bool lock_request();
            bool unlock_request();
            bool try_lock();
            boost::unique_ptr<Mutex> processes();
            bool destruction();
        private:
            Mutex::mutex_t * mutex;
    };

}

#endif /* CONTROLLER__SEMAPHORE_CONTROLLER_HPP_ */
