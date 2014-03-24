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

#include "threadsynocl/monitor_controller.hpp"
#include "utils/base/util_thread.hpp"
#include "utils/baes/platformsocket.hpp"

#include <boost/scoped_ptr.hpp>

#include <assert.h>

#include <iostream>

#include <pthread.h>


#define BASE_UNUSED_VARIABLE(x) ((void)(x))

namespace controller
{
    using boost::scoped_ptr;

    /**
     * monitor_controller implementation using the POSIX pthread library
     *
     * @version $Id:$
     */
    class monitor_controller::impl
    {

        public:

            impl()
                : ownedMutex_(new mutex_controller()),
                  mutex_(NULL),
                  condInitialized_(false) {
                init(ownedMutex_.get());
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
             * Exception-throwing version of waitForTimeRelative(), called simply
             * wait(int64) for historical reasons.  Timeout is in milliseconds.
             *
             * If the condition occurs,  this function returns cleanly; on timeout or
             * error an exception is thrown.
             */
            void wait(int64_t timeout_ms) const {
                int result = waitForTimeRelative(timeout_ms);

                if (result == BASE_ETIMEDOUT) {
                    // pthread_cond_timedwait has been observed to return early on
                    // various platforms, so comment out this assert.
                    //assert(util_thread::currentTime() >= (now + timeout));
                    throw TimedOutException();
                } else if (result != 0) {
                    throw TException(
                            "pthread_cond_wait() or pthread_cond_timedwait() failed");
                }
            }

            /**
             * Waits until the specified timeout in milliseconds for the condition to
             * occur, or waits forever if timeout_ms == 0.
             *
             * Returns 0 if condition occurs, BASE_ETIMEDOUT on timeout, or an error code.
             */
            int waitForTimeRelative(int64_t timeout_ms) const {
                if (timeout_ms == 0LL) {
                    return waitForever();
                }

                struct BASE_TIMESPEC abstime;

                util_thread::toTimespec(abstime, util_thread::currentTime() + timeout_ms);

                return waitForTime(&abstime);
            }

            /**
             * Waits until the absolute time specified using struct BASE_TIMESPEC.
             * Returns 0 if condition occurs, BASE_ETIMEDOUT on timeout, or an error code.
             */
            int waitForTime(const BASE_TIMESPEC *abstime) const {
                assert(mutex_);
                pthread_mutex_t *muteximpl =
                        reinterpret_cast<pthread_mutex_t *>(mutex_->getUnderlyingimpl());
                assert(muteximpl);

                // XXX Need to assert that caller owns mutex
                return pthread_cond_timedwait(&pthread_cond_,
                        muteximpl,
                        abstime);
            }

            int waitForTime(const struct timeval *abstime) const {
                struct BASE_TIMESPEC temp;
                temp.tv_sec  = abstime->tv_sec;
                temp.tv_nsec = abstime->tv_usec * 1000;
                return waitForTime(&temp);
            }
            /**
             * Waits forever until the condition occurs.
             * Returns 0 if condition occurs, or an error code otherwise.
             */
            int waitForever() const {
                assert(mutex_);
                pthread_mutex_t *muteximpl =
                        reinterpret_cast<pthread_mutex_t *>(mutex_->getUnderlyingimpl());
                assert(muteximpl);
                return pthread_cond_wait(&pthread_cond_, muteximpl);
            }


            void notify() {
                // XXX Need to assert that caller owns mutex
                int iret = pthread_cond_signal(&pthread_cond_);
                BASE_UNUSED_VARIABLE(iret);
                assert(iret == 0);
            }

            void notifyAll() {
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
                    throw SystemResourceException();
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

            scoped_ptr<mutex_controller> ownedMutex_;
            mutex_controller *mutex_;

            mutable pthread_cond_t pthread_cond_;
            mutable bool condInitialized_;
    };

    monitor_controller::monitor_controller() : impl_(new monitor_controller::impl()) {}
    monitor_controller::monitor_controller(Mutex *mutex) : impl_(new monitor_controller::impl(mutex)) {}
    monitor_controller::monitor_controller(monitor_controller *monitor) : impl_(new monitor_controller::impl(monitor)) {}

    monitor_controller::~Monitor()
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

    int monitor_controller::waitForTime(const BASE_TIMESPEC *abstime) const
    {
        return impl_->waitForTime(abstime);
    }

    int monitor_controller::waitForTime(const timeval *abstime) const
    {
        return impl_->waitForTime(abstime);
    }

    int monitor_controller::waitForTimeRelative(int64_t timeout_ms) const
    {
        return impl_->waitForTimeRelative(timeout_ms);
    }

    int monitor_controller::waitForever() const
    {
        return impl_->waitForever();
    }

    void monitor_controller::notify() const
    {
        impl_->notify();
    }

    void monitor_controller::notifyAll() const
    {
        impl_->notifyAll();
    }

}// controller
