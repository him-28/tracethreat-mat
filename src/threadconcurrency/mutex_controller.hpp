#ifndef CONTROLLER__MUTEX_CONTROLLER_HPP
#define CONTROLLER__MUTEX_CONTROLLER_HPP

/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                                     Authors	          Date
 * - Thrift contributed in apache version 2.0 
 * - Controller::mutex_controller supported thread_manager. Thread_manager for read buffer send to CPU 
 *   scans virus.                                                  R.Chatsiri         25/03/2014
 */


#include <vector>


#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>



//
#include "utils/logger/clutil_logger.hpp"

namespace controller
{

    /*Mutex */
#ifndef HNMAV_NO_CONTENTION_PROFILING

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
    typedef void (*mutex_wait_callback)(const void *id, int64_t  wait_time_macros);
    void enable_mutex_profiling(int32_t profiling_sample_rate,
            mutex_wait_callback callback);

#endif

    /**
     * A simple mutex class
     *
     * @version $Id:$
     */
    class mutex_controller
    {
        public:
            typedef void (*initializer)(void *);

            mutex_controller(initializer init = DEFAULT_INITIALIZER);
            virtual ~mutex_controller() {}
            virtual void lock() const;
            virtual bool try_lock() const;
            virtual bool timed_lock(int64_t milliseconds) const;
            virtual void unlock() const;

						void * get_underlying_impl() const;

            static void DEFAULT_INITIALIZER(void *);
            static void ADAPTIVE_INITIALIZER(void *);
            static void RECURSIVE_INITIALIZER(void *);

        private:

            class impl;
            boost::shared_ptr<impl> impl_;
    };

    /* end of Mutex */

		/*RW Mutex */
  class read_write_mutex
    {
        public:
            read_write_mutex();
            virtual ~read_write_mutex() {}

            // these get the lock and block until it is done successfully
            virtual void acquire_read() const;
            virtual void acquire_write() const;

            // these attempt to get the lock, returning false immediately if they fail
            virtual bool attempt_read() const;
            virtual bool attempt_write() const;

            // this releases both read and write locks
            virtual void release() const;

        private:

            class impl;
            boost::shared_ptr<impl> impl_;
    };

    /**
     * A ReadWriteMutex that guarantees writers will not be starved by readers:
     * When a writer attempts to acquire the mutex, all new readers will be
     * blocked from acquiring the mutex until the writer has acquired and
     * released it. In some operating systems, this may already be guaranteed
     * by a regular ReadWriteMutex.
     */
    class nostarve_readwrite_mutex  : public read_write_mutex
    {
        public:
						nostarve_readwrite_mutex();
            virtual void acquire_read() const;
            virtual void acquire_write() const;

        private:
            mutex_controller mutex_;
            mutable volatile bool  write_waiting_;
    };

    class guard : boost::noncopyable
    {
        public:
            guard(const mutex_controller& value, int64_t timeout = 0) : mutex_(&value) {
                if (timeout == 0) {
                    value.lock();
                } else if (timeout < 0) {
                    if (!value.try_lock()) {
                        mutex_ = NULL;
                    }
                } else {
                    if (!value.timed_lock(timeout)) {
                        mutex_ = NULL;
                    }
                }
            }
            ~guard() {
                if (mutex_) {
                    mutex_->unlock();
                }
            }

            operator bool() const {
                return (mutex_ != NULL);
            }

        private:
            const mutex_controller *mutex_;
    };
		/*
    // Can be used as second argument to RWGuard to make code more readable
    // as to whether we're doing acquireRead() or acquireWrite().
    enum RWGuardType {
        RW_READ = 0,
        RW_WRITE = 1
    };


    class RWGuard : boost::noncopyable
    {
        public:
            RWGuard(const ReadWriteMutex& value, bool write = false)
                : rw_mutex_(value) {
                if (write) {
                    rw_mutex_.acquireWrite();
                } else {
                    rw_mutex_.acquireRead();
                }
            }

            RWGuard(const ReadWriteMutex& value, RWGuardType type)
                : rw_mutex_(value) {
                if (type == RW_WRITE) {
                    rw_mutex_.acquireWrite();
                } else {
                    rw_mutex_.acquireRead();
                }
            }
            ~RWGuard() {
                rw_mutex_.release();
            }
        private:
            const ReadWriteMutex& rw_mutex_;
    };
	*/


}

#endif /* CONTROLLER__SEMAPHORE_CONTROLLER_HPP_ */
