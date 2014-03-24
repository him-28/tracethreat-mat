#ifndef CONTROLLER__SEMAPHORE_CONTROLLER_HPP_
#define CONTROLLER__SEMAPHORE_CONTROLLER_HPP_

#include <vector>


#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>



//
#include "utils/logger/clutil_logger.hpp"

namespace controller
{
    //work around with h_ prefix
    namespace h_util = hnmav_util;
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

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

    /*Mutex */
#ifndef THRIFT_NO_CONTENTION_PROFILING

    /**
     * Determines if the Thrift Mutex and ReadWriteMutex classes will attempt to
     * profile their blocking acquire methods. If this value is set to non-zero,
     * Thrift will attempt to invoke the callback once every profilingSampleRate
     * times.  However, as the sampling is not synchronized the rate is not
     * guranateed, and could be subject to big bursts and swings.  Please ensure
     * your sampling callback is as performant as your application requires.
     *
     * The callback will get called with the wait time taken to lock the mutex in
     * usec and a (void*) that uniquely identifies the Mutex (or ReadWriteMutex)
     * being locked.
     *
     * The enableMutexProfiling() function is unsynchronized; calling this function
     * while profiling is already enabled may result in race conditions.  On
     * architectures where a pointer assignment is atomic, this is safe but there
     * is no guarantee threads will agree on a single callback within any
     * particular time period.
     */
    typedef void (*MutexWaitCallback)(const void *id, int64_t waitTimeMicros);
    void enableMutexProfiling(int32_t profilingSampleRate,
            MutexWaitCallback callback);

#endif

    /**
     * A simple mutex class
     *
     * @version $Id:$
     */
    class Mutex
    {
        public:
            typedef void (*Initializer)(void *);

            Mutex(Initializer init = DEFAULT_INITIALIZER);
            virtual ~Mutex() {}
            virtual void lock() const;
            virtual bool trylock() const;
            virtual bool timedlock(int64_t milliseconds) const;
            virtual void unlock() const;

            void *getUnderlyingImpl() const;

            static void DEFAULT_INITIALIZER(void *);
            static void ADAPTIVE_INITIALIZER(void *);
            static void RECURSIVE_INITIALIZER(void *);

        private:

            class impl;
            boost::shared_ptr<impl> impl_;
    };

    /* end of Mutex */

}

#endif /* CONTROLLER__SEMAPHORE_CONTROLLER_HPP_ */
