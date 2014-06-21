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
 *  - Thirft concurrency licenses unders Apahce License Version 2.0
 *  - Mutex support semaphore_controller.hpp                     R.chatsiri       24/03/2014 
 */

#define BASE_UNUSED_VARIABLE(x) ((void)(x))

#include "threadconcurrency/monitor_controller.hpp"

#include "threadconcurrency/util_thread.hpp"

#include <boost/scoped_ptr.hpp>

#include <assert.h>

#include <iostream>

#include <pthread.h>



//#include <sys/errno.h>

namespace controller
{
    using boost::scoped_ptr;
		namespace exceptions = hnmav_exception::controller;
    /**
     * monitor_controller implementation using the POSIX pthread library
     *
     * @version $Id:$
     */
    class monitor_controller::impl
    {

        public:

            impl()
                : owned_mutex_(new mutex_controller()),
                  mutex_(NULL),
                  condInitialized_(false) {
                init(owned_mutex_.get());
            }

            impl(mutex_controller *mutex)
                : mutex_(NULL),
                  condInitialized_(false) {
                init(mutex);
            }

            impl(monitor_controller *monitor)
                : mutex_(NULL),
                  condInitialized_(false) {
                init(&(monitor->mutex()));
            }

            ~impl() {
                cleanup();
            }

            mutex_controller& mutex() {
                return *mutex_;
            }
            void lock() {
                mutex().lock();
            }
            void unlock() {
                mutex().unlock();
            }

            /**
             * Exception-throwing version of wait_for_time_relative(), called simply
             * wait(int64) for historical reasons.  Timeout is in milliseconds.
             *
             * If the condition occurs,  this function returns cleanly; on timeout or
             * error an exception is thrown.
             */
            void wait(int64_t timeout_ms) const {
                int result = wait_for_time_relative(timeout_ms);

                if (result == ETIMEDOUT) {
                    // pthread_cond_timedwait has been observed to return early on
                    // various platforms, so comment out this assert.
                    //assert(util_thread::current_time() >= (now + timeout));
                    throw exceptions::timed_out_exception();//TimedOutException();
                } else if (result != 0) {
                    throw exceptions::t_exception(
                            "pthread_cond_wait() or pthread_cond_timedwait() failed");
                }
            }

            /**
             * Waits until the specified timeout in milliseconds for the condition to
             * occur, or waits forever if timeout_ms == 0.
             *
             * Returns 0 if condition occurs, BASE_ETIMEDOUT on timeout, or an error code.
             */
            int wait_for_time_relative(int64_t timeout_ms) const {
                if (timeout_ms == 0LL) {
                    return wait_forever();
                }

                struct TIMESPEC abstime;

                util_thread::to_time_spec(abstime, util_thread::current_time() + timeout_ms);

                return wait_for_time(&abstime);
            }

            /**
             * Waits until the absolute time specified using struct BASE_TIMESPEC.
             * Returns 0 if condition occurs, BASE_ETIMEDOUT on timeout, or an error code.
             */
            int wait_for_time(const TIMESPEC *abstime) const {
                assert(mutex_);
                pthread_mutex_t *muteximpl =
                        reinterpret_cast<pthread_mutex_t *>(mutex_->get_underlying_impl());
                assert(muteximpl);

                // XXX Need to assert that caller owns mutex
                return pthread_cond_timedwait(&pthread_cond_,
                        muteximpl,
                        abstime);
            }

            int wait_for_time(const struct timeval *abstime) const {
                struct TIMESPEC temp;
                temp.tv_sec  = abstime->tv_sec;
                temp.tv_nsec = abstime->tv_usec * 1000;
                return wait_for_time(&temp);
            }
            /**
             * Waits forever until the condition occurs.
             * Returns 0 if condition occurs, or an error code otherwise.
             */
            int wait_forever() const {
                assert(mutex_);
                pthread_mutex_t *muteximpl =
                        reinterpret_cast<pthread_mutex_t *>(mutex_->get_underlying_impl());
                assert(muteximpl);
                return pthread_cond_wait(&pthread_cond_, muteximpl);
            }


            void notify() {
                // XXX Need to assert that caller owns mutex
                int iret = pthread_cond_signal(&pthread_cond_);
                BASE_UNUSED_VARIABLE(iret);
                assert(iret == 0);
            }

            void notify_all() {
                // XXX Need to assert that caller owns mutex
                int iret = pthread_cond_broadcast(&pthread_cond_);
                BASE_UNUSED_VARIABLE(iret);
                assert(iret == 0);
            }

        private:

            void init(mutex_controller *mutex) {
                mutex_ = mutex;

                if (pthread_cond_init(&pthread_cond_, NULL) == 0) {
                    condInitialized_ = true;
                }

                if (!condInitialized_) {
                    cleanup();
                    throw exceptions::system_resource_exception();
                }
            }

            void cleanup() {
                if (condInitialized_) {
                    condInitialized_ = false;
                    int iret = pthread_cond_destroy(&pthread_cond_);
                    BASE_UNUSED_VARIABLE(iret);
                    assert(iret == 0);
                }
            }

            scoped_ptr<mutex_controller> owned_mutex_;
            mutex_controller *mutex_;

            mutable pthread_cond_t pthread_cond_;
            mutable bool condInitialized_;
    };

    monitor_controller::monitor_controller() 
			: impl_(new monitor_controller::impl()) {}
    monitor_controller::monitor_controller(mutex_controller *mutex) 
			: impl_(new monitor_controller::impl(mutex)) {}
    monitor_controller::monitor_controller(monitor_controller *monitor) 
			: impl_(new monitor_controller::impl(monitor)) {}

    monitor_controller::~monitor_controller()
    {
        delete impl_;
    }

    mutex_controller& monitor_controller::mutex() const
    {
        return impl_->mutex();
    }

    void monitor_controller::lock() const
    {
        impl_->lock();
    }

    void monitor_controller::unlock() const
    {
        impl_->unlock();
    }

    void monitor_controller::wait(int64_t timeout) const
    {
        impl_->wait(timeout);
    }

    int monitor_controller::wait_for_time(const TIMESPEC *abstime) const
    {
        return impl_->wait_for_time(abstime);
    }

    int monitor_controller::wait_for_time(const timeval *abstime) const
    {
        return impl_->wait_for_time(abstime);
    }

    int monitor_controller::wait_for_time_relative(int64_t timeout_ms) const
    {
        return impl_->wait_for_time_relative(timeout_ms);
    }

    int monitor_controller::wait_forever() const
    {
        return impl_->wait_forever();
    }

    void monitor_controller::notify() const
    {
        impl_->notify();
    }

    void monitor_controller::notify_all() const
    {
        impl_->notify_all();
    }

}// controller

