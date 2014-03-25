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
 * - Semaphore define																						Chatsiri.rat      01/10/2013
 */
#include "threadsyncocl/semaphore_controller.hpp"

#include "utils/base/util_thread.hpp"


namespace controller
{

    template<typename mutex_controller>
    bool mutex_buffer<mutex_controller>::init()
    {
        // logger
        logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();


        if(mx_ptr_vec.size() == 0) {
            mx_ptr ptr(new mutex_controller);
            mutex_controller   *m = ptr.get();

            if(!pthread_mutex_init(&m->mutex_t, NULL)) {
                mx_ptr_vec.push_back(ptr);
                return true;
            }
        } else {
            mx_ptr_vec.clear();
            mx_ptr ptr(new mutex_controller);
            mx_ptr_vec.push_back(ptr);
            return true;
        }

        return false;
    }

    template<typename mutex_controller>
    bool mutex_buffer<mutex_controller>::lock_request()
    {
        logger->write_info(" Lock request ");
        boost::shared_ptr<mutex_controller> mx = mx_ptr_vec.back();
        mutex_controller *m = mx.get();

        if(!pthread_mutex_lock(&m->mutex_t)) {
            return true;
        }

        return false;
    }

    template<typename mutex_controller>
    bool mutex_buffer<mutex_controller>::unlock_request()
    {
        logger->write_info(" Unlock request ");
        boost::shared_ptr<mutex_controller> mx = mx_ptr_vec.back();
        mutex_controller *m = mx.get();

        if(!pthread_mutex_unlock(&m->mutex_t)) {
            return true;
        }

        return false;
    }

    template<typename mutex_controller>
    bool mutex_buffer<mutex_controller>::try_lock()
    {


    }

    template<typename mutex_controller>
    boost::shared_ptr<mutex_controller> mutex_buffer<Mutex>::processes()
    {
        return mx_ptr_vec.back();
    }

    template<typename mutex_controller>
    bool mutex_buffer<mutex_controller>::destruction()
    {

        boost::shared_ptr<mutex_controller> mx = mx_ptr_vec.back();
        mutex_controller *m = mx.get();

        if(!pthread_mutex_destroy(&m->mutex_t)) {
            return true;
        }

        return false;
    }


    template class mutex_buffer<mutex_controller>;


    /* mutex declared */
#ifndef BASE_NO_CONTENTION_PROFILING

    static sig_atomic_t mutex_profiliing_sample_rate = 0;
    static mutex_wait_callback mutex_profiling_callback  = 0;

    volatile static sig_atomic_t mutex_profiling_counter = 0;

    void enable_mutex_profiling(int32_t profiling_sample_rate,
            mutex_wait_callback callback)
    {
        mutex_profiling_sample_rate = profiling_sample_rate;
        mutex_profiling_callback    = callback;
    }

#define PROFILE_MUTEX_START_LOCK() \
    int64_t _lock_startTime =   maybe_get_profiling_start_time();

#define PROFILE_MUTEX_NOT_LOCKED() \
  do { \
    if (_lock_startTime > 0) { \
      int64_t endTime = util_thread::current_time_usec(); \
      (*mutex_profiling_callback)(this, endTime - _lock_startTime); \
    } \
  } while (0)

#define PROFILE_MUTEX_LOCKED() \
  do { \
    profileTime_ = _lock_startTime; \
    if (profileTime_ > 0) { \
      profileTime_ = util_thread::current_time_usec()  - profileTime_; \
    } \
  } while (0)

#define PROFILE_MUTEX_START_UNLOCK() \
  int64_t _temp_profileTime = profileTime_; \
  profileTime_ = 0;

#define PROFILE_MUTEX_UNLOCKED() \
  do { \
    if (_temp_profileTime > 0) { \
      (*mutex_profiling_callback)(this, _temp_profileTime); \
    } \
  } while (0)

    static inline int64_t maybe_get_profiling_start_time()
    {
        if (mutex_profiling_sample_rate && mutex_profiling_callback) {
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
            sig_atomic_t localValue = --mutex_profiling_counter;

            if (localValue <= 0) {
                mutex_profiling_counter = mutex_profiling_sample_rate;
                return util_thread::current_time_usec();
            }
        }

        return 0;
    }

#else
#  define PROFILE_MUTEX_START_LOCK()
#  define PROFILE_MUTEX_NOT_LOCKED()
#  define PROFILE_MUTEX_LOCKED()
#  define PROFILE_MUTEX_START_UNLOCK()
#  define PROFILE_MUTEX_UNLOCKED()
#endif // BASE_NO_CONTENTION_PROFILING

    /**
     * Implementation of mutex_controller class using POSIX mutex
     *
     * @version $Id:$
     */
    class mutex_controller::impl
    {
        public:
            impl(initializer init) : initialized_(false) {
#ifndef BASE_NO_CONTENTION_PROFILING
                profileTime_ = 0;
#endif
                init(&pthread_mutex_);
                initialized_ = true;
            }

            ~impl() {
                if (initialized_) {
                    initialized_ = false;
                    int ret = pthread_mutex_destroy(&pthread_mutex_);
                    BASE_UNUSED_VARIABLE(ret);
                    assert(ret == 0);
                }
            }

            void lock() const {
                PROFILE_MUTEX_START_LOCK();
                pthread_mutex_lock(&pthread_mutex_);
                PROFILE_MUTEX_LOCKED();
            }

            bool try_lock() const {
                return (0 == pthread_mutex_trylock(&pthread_mutex_));
            }

            bool timed_lock(int64_t milliseconds) const {
#if defined(_POSIX_TIMEOUTS) && _POSIX_TIMEOUTS >= 200112L
                PROFILE_MUTEX_START_LOCK();

                struct BASE_TIMESPEC ts;

                util_thread::to_time_spec(ts, milliseconds + util_thread::current_time());

                int ret = pthread_mutex_timedlock(&pthread_mutex_, &ts);

                if (ret == 0) {
                    PROFILE_MUTEX_LOCKED();
                    return true;
                }

                PROFILE_MUTEX_NOT_LOCKED();
                return false;
#else
                /* Otherwise follow solution used by Mono for Android */
                struct BASE_TIMESPEC sleepytime, now, to;

                /* This is just to avoid a completely busy wait */
                sleepytime.tv_sec = 0;
                sleepytime.tv_nsec = 10000000L; /* 10ms */

                util_thread::to_time_spec(to, milliseconds + util_thread::current_time());

                while ((try_lock()) == false) {
                    util_thread::to_time_spec(now, util_thread::current_time());

                    if (now.tv_sec >= to.tv_sec && now.tv_nsec >= to.tv_nsec) {
                        return false;
                    }

                    nanosleep(&sleepytime, NULL);
                }

                return true;
#endif
            }

            void unlock() const {
                PROFILE_MUTEX_START_UNLOCK();
                pthread_mutex_unlock(&pthread_mutex_);
                PROFILE_MUTEX_UNLOCKED();
            }

						void *get_underlying_impl() const{
                return (void *) &pthread_mutex_;
            }

        private:
            mutable pthread_mutex_t pthread_mutex_;
            mutable bool initialized_;
#ifndef BASE_NO_CONTENTION_PROFILING
            mutable int64_t profileTime_;
#endif
    };

    mutex_controller::mutex_controller(initializer init) : impl_(new mutex_controller::impl(init)) {}

    void *mutex_controller::get_underlying_impl() const
    {
        return impl_->get_underlying_impl();
    }

    void mutex_controller::lock() const
    {
        impl_->lock();
    }

    bool mutex_controller::try_lock() const
    {
        return impl_->trylock();
    }

    bool mutex_controller::timed_lock(int64_t ms) const
    {
        return impl_->timedlock(ms);
    }

    void mutex_controller::unlock() const
    {
        impl_->unlock();
    }

    void mutex_controller::DEFAULT_INITIALIZER(void *arg)
    {
        pthread_mutex_t *pthread_mutex = (pthread_mutex_t *)arg;
        int ret = pthread_mutex_init(pthread_mutex, NULL);
        THRIFT_UNUSED_VARIABLE(ret);
        assert(ret == 0);
    }

#if defined(PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP) || defined(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
    static void init_with_kind(pthread_mutex_t *mutex, int kind)
    {
        pthread_mutexattr_t mutexattr;
        int ret = pthread_mutexattr_init(&mutexattr);
        assert(ret == 0);

        // Apparently, this can fail.  Should we really be aborting?
        ret = pthread_mutexattr_settype(&mutexattr, kind);
        assert(ret == 0);

        ret = pthread_mutex_init(mutex, &mutexattr);
        assert(ret == 0);

        ret = pthread_mutexattr_destroy(&mutexattr);
        assert(ret == 0);
        THRIFT_UNUSED_VARIABLE(ret);
    }
#endif

#ifdef PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP
    void mutex_controller::ADAPTIVE_INITIALIZER(void *arg)
    {
        // From mysql source: mysys/my_thr_init.c
        // Set mutex type to "fast" a.k.a "adaptive"
        //
        // In this case the thread may steal the mutex from some other thread
        // that is waiting for the same mutex. This will save us some
        // context switches but may cause a thread to 'starve forever' while
        // waiting for the mutex (not likely if the code within the mutex is
        // short).
        init_with_kind((pthread_mutex_t *)arg, PTHREAD_MUTEX_ADAPTIVE_NP);
    }
#endif

#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
    void mutex_controller::RECURSIVE_INITIALIZER(void *arg)
    {
        init_with_kind((pthread_mutex_t *)arg, PTHREAD_MUTEX_RECURSIVE_NP);
    }
#endif




    /* end mutex declared */

}
