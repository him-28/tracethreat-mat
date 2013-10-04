#ifndef CONTROLLER__SEMAPHORE_CONTROLLER_HPP_
#define CONTROLLER__SEMAPHORE_CONTROLLER_HPP_

#include <vector>


#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/shared_ptr.hpp>

namespace controller
{
    //Define type class
    class Mutex
    {
        public:
            pthread_mutex_t mutex_t;
    };

    // Bule print of mutex.
    template<typename SemaphoreProcesses>
    class semaphore
    {
        public:
            virtual bool lock_request()   = 0;
            virtual bool unlock_request() = 0;
            virtual boost::shared_ptr<SemaphoreProcesses> processes() = 0;
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
            boost::shared_ptr<Mutex> processes();
            bool destruction();
        private:
						typedef boost::shared_ptr<Mutex>  mx_ptr;
            //move sementics
            std::vector<boost::shared_ptr<Mutex> >   mx_ptr_vec;
    };
	
}

#endif /* CONTROLLER__SEMAPHORE_CONTROLLER_HPP_ */