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


//#include <thrift/thrift-config.h>

//#include <thrift/Thrift.h>
//#include <thrift/concurrency/Mutex.h>
//#include <thrift/concurrency/Util.h>




#include "threadsyncocl/semaphore_controller.hpp"
#include "utils/base/util_thread.hpp"

#include <assert.h>
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include <signal.h>

using boost::shared_ptr;

namespace controller
{
    /* macro decleared */

#ifndef THRIFT_NO_CONTENTION_PROFILING

    static sig_atomic_t mutexProfilingSampleRate = 0;
    static MutexWaitCallback mutexProfilingCallback = 0;

    volatile static sig_atomic_t mutexProfilingCounter = 0;

    void enableMutexProfiling(int32_t profilingSampleRate,
            MutexWaitCallback callback)
    {
        mutexProfilingSampleRate = profilingSampleRate;
        mutexProfilingCallback = callback;
    }

#define PROFILE_MUTEX_START_LOCK() \
    int64_t _lock_startTime = maybeGetProfilingStartTime();

#define PROFILE_MUTEX_NOT_LOCKED() \
  do { \
    if (_lock_startTime > 0) { \
      int64_t endTime = Util::currentTimeUsec(); \
      (*mutexProfilingCallback)(this, endTime - _lock_startTime); \
    } \
  } while (0)

#define PROFILE_MUTEX_LOCKED() \
  do { \
    profileTime_ = _lock_startTime; \
    if (profileTime_ > 0) { \
      profileTime_ = Util::currentTimeUsec() - profileTime_; \
    } \
  } while (0)

#define PROFILE_MUTEX_START_UNLOCK() \
  int64_t _temp_profileTime = profileTime_; \
  profileTime_ = 0;

#define PROFILE_MUTEX_UNLOCKED() \
  do { \
    if (_temp_profileTime > 0) { \
      (*mutexProfilingCallback)(this, _temp_profileTime); \
    } \
  } while (0)

    static inline int64_t maybeGetProfilingStartTime()
    {
        if (mutexProfilingSampleRate && mutexProfilingCallback) {
            // This block is unsynchronized, but should produce a reasonable sampling
            // rate on most architectures.  The main race conditions are the gap
            // between the decrement and the test, the non-atomicity of decrement, and
            // potential caching of different values at different CPUs.
            //
            // - if two decrements race, the likeliest result is that the counter
            //      decrements slowly (perhaps much more slowly) than intended.
            //
            // - many threads could potentially decrement before resetting the counter
            //      to its large value, causing each additional incoming thread to
            //      profile every call.  This situation is unlikely to persist for long
            //      as the critical gap is quite short, but profiling could be bursty.
            sig_atomic_t localValue = --mutexProfilingCounter;

            if (localValue <= 0) {
                mutexProfilingCounter = mutexProfilingSampleRate;
                return Util::currentTimeUsec();
            }
        }

        return 0;
    }

#else



    /* end macro declared */



    /**
     * Implementation of ReadWriteMutex class using POSIX rw lock
     *
     * @version $Id:$
     */
    class ReadWriteMutex::impl
    {
        public:
            impl() : initialized_(false) {
#ifndef THRIFT_NO_CONTENTION_PROFILING
                profileTime_ = 0;
#endif
                int ret = pthread_rwlock_init(&rw_lock_, NULL);
                THRIFT_UNUSED_VARIABLE(ret);
                assert(ret == 0);
                initialized_ = true;
            }

            ~impl() {
                if(initialized_) {
                    initialized_ = false;
                    int ret = pthread_rwlock_destroy(&rw_lock_);
                    THRIFT_UNUSED_VARIABLE(ret);
                    assert(ret == 0);
                }
            }

            void acquireRead() const {
                PROFILE_MUTEX_START_LOCK();
                pthread_rwlock_rdlock(&rw_lock_);
                PROFILE_MUTEX_NOT_LOCKED();  // not exclusive, so use not-locked path
            }

            void acquireWrite() const {
                PROFILE_MUTEX_START_LOCK();
                pthread_rwlock_wrlock(&rw_lock_);
                PROFILE_MUTEX_LOCKED();
            }

            bool attemptRead() const {
                return !pthread_rwlock_tryrdlock(&rw_lock_);
            }

            bool attemptWrite() const {
                return !pthread_rwlock_trywrlock(&rw_lock_);
            }

            void release() const {
                PROFILE_MUTEX_START_UNLOCK();
                pthread_rwlock_unlock(&rw_lock_);
                PROFILE_MUTEX_UNLOCKED();
            }

        private:
            mutable pthread_rwlock_t rw_lock_;
            mutable bool initialized_;
#ifndef THRIFT_NO_CONTENTION_PROFILING
            mutable int64_t profileTime_;
#endif
    };

    ReadWriteMutex::ReadWriteMutex() : impl_(new ReadWriteMutex::impl()) {}

    void ReadWriteMutex::acquireRead() const
    {
        impl_->acquireRead();
    }

    void ReadWriteMutex::acquireWrite() const
    {
        impl_->acquireWrite();
    }

    bool ReadWriteMutex::attemptRead() const
    {
        return impl_->attemptRead();
    }

    bool ReadWriteMutex::attemptWrite() const
    {
        return impl_->attemptWrite();
    }

    void ReadWriteMutex::release() const
    {
        impl_->release();
    }

    NoStarveReadWriteMutex::NoStarveReadWriteMutex() : writerWaiting_(false) {}

    void NoStarveReadWriteMutex::acquireRead() const
    {
        if (writerWaiting_) {
            // writer is waiting, block on the writer's mutex until he's done with it
            mutex_.lock();
            mutex_.unlock();
        }

        ReadWriteMutex::acquireRead();
    }

    void NoStarveReadWriteMutex::acquireWrite() const
    {
        // if we can acquire the rwlock the easy way, we're done
        if (attemptWrite()) {
            return;
        }

        // failed to get the rwlock, do it the hard way:
        // locking the mutex and setting writerWaiting will cause all new readers to
        // block on the mutex rather than on the rwlock.
        mutex_.lock();
        writerWaiting_ = true;
        ReadWriteMutex::acquireWrite();
        writerWaiting_ = false;
        mutex_.unlock();
    }

}// end of namespace controller


