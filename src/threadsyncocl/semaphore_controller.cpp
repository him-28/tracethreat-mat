
/* 						Titles														Authors 												Date
 * - Semaphore define														Chatsiri.rat                    01/10/2013
 */
#include "threadsyncocl/semaphore_controller.hpp"

#include "utils/base/util_thread.hpp"


namespace controller
{

    template<typename Mutex>
    bool mutex_buffer<Mutex>::init()
    {
        // logger
        logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();


        if(mx_ptr_vec.size() == 0) {
            mx_ptr ptr(new Mutex);
            Mutex   *m = ptr.get();

            if(!pthread_mutex_init(&m->mutex_t, NULL)) {
                mx_ptr_vec.push_back(ptr);
                return true;
            }
        } else {
            mx_ptr_vec.clear();
            mx_ptr ptr(new Mutex);
            mx_ptr_vec.push_back(ptr);
            return true;
        }

        return false;
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::lock_request()
    {
        logger->write_info(" Lock request ");
        boost::shared_ptr<Mutex> mx = mx_ptr_vec.back();
        Mutex *m = mx.get();

        if(!pthread_mutex_lock(&m->mutex_t)) {
            return true;
        }

        return false;
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::unlock_request()
    {
        logger->write_info(" Unlock request ");
        boost::shared_ptr<Mutex> mx = mx_ptr_vec.back();
        Mutex *m = mx.get();

        if(!pthread_mutex_unlock(&m->mutex_t)) {
            return true;
        }

        return false;
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::try_lock()
    {


    }

    template<typename Mutex>
    boost::shared_ptr<Mutex> mutex_buffer<Mutex>::processes()
    {
        return mx_ptr_vec.back();
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::destruction()
    {

        boost::shared_ptr<Mutex> mx = mx_ptr_vec.back();
        Mutex *m = mx.get();

        if(!pthread_mutex_destroy(&m->mutex_t)) {
            return true;
        }

        return false;
    }


    template class mutex_buffer<Mutex>;


    /* mutex declared */
#ifndef HNMAV_NO_CONTENTION_PROFILING

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
#  define PROFILE_MUTEX_START_LOCK()
#  define PROFILE_MUTEX_NOT_LOCKED()
#  define PROFILE_MUTEX_LOCKED()
#  define PROFILE_MUTEX_START_UNLOCK()
#  define PROFILE_MUTEX_UNLOCKED()
#endif // HNMAV_NO_CONTENTION_PROFILING

    /**
     * Implementation of Mutex class using POSIX mutex
     *
     * @version $Id:$
     */
    class Mutex::impl
    {
        public:
            impl(Initializer init) : initialized_(false) {
#ifndef HNMAV_NO_CONTENTION_PROFILING
                profileTime_ = 0;
#endif
                init(&pthread_mutex_);
                initialized_ = true;
            }

            ~impl() {
                if (initialized_) {
                    initialized_ = false;
                    int ret = pthread_mutex_destroy(&pthread_mutex_);
                    THRIFT_UNUSED_VARIABLE(ret);
                    assert(ret == 0);
                }
            }

            void lock() const {
                PROFILE_MUTEX_START_LOCK();
                pthread_mutex_lock(&pthread_mutex_);
                PROFILE_MUTEX_LOCKED();
            }

            bool trylock() const {
                return (0 == pthread_mutex_trylock(&pthread_mutex_));
            }

            bool timedlock(int64_t milliseconds) const {
#if defined(_POSIX_TIMEOUTS) && _POSIX_TIMEOUTS >= 200112L
                PROFILE_MUTEX_START_LOCK();

                struct HNMAV_TIMESPEC ts;
                Util::toTimespec(ts, milliseconds + Util::currentTime());
                int ret = pthread_mutex_timedlock(&pthread_mutex_, &ts);

                if (ret == 0) {
                    PROFILE_MUTEX_LOCKED();
                    return true;
                }

                PROFILE_MUTEX_NOT_LOCKED();
                return false;
#else
                /* Otherwise follow solution used by Mono for Android */
                struct HNMAV_TIMESPEC sleepytime, now, to;

                /* This is just to avoid a completely busy wait */
                sleepytime.tv_sec = 0;
                sleepytime.tv_nsec = 10000000L; /* 10ms */

                Util::toTimespec(to, milliseconds + Util::currentTime());

                while ((trylock()) == false) {
                    Util::toTimespec(now, Util::currentTime());

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

            void *getUnderlyingImpl() const {
                return (void *) &pthread_mutex_;
            }

        private:
            mutable pthread_mutex_t pthread_mutex_;
            mutable bool initialized_;
#ifndef HNMAV_NO_CONTENTION_PROFILING
            mutable int64_t profileTime_;
#endif
    };

    Mutex::Mutex(Initializer init) : impl_(new Mutex::impl(init)) {}

    void *Mutex::getUnderlyingImpl() const
    {
        return impl_->getUnderlyingImpl();
    }

    void Mutex::lock() const
    {
        impl_->lock();
    }

    bool Mutex::trylock() const
    {
        return impl_->trylock();
    }

    bool Mutex::timedlock(int64_t ms) const
    {
        return impl_->timedlock(ms);
    }

    void Mutex::unlock() const
    {
        impl_->unlock();
    }

    void Mutex::DEFAULT_INITIALIZER(void *arg)
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
    void Mutex::ADAPTIVE_INITIALIZER(void *arg)
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
    void Mutex::RECURSIVE_INITIALIZER(void *arg)
    {
        init_with_kind((pthread_mutex_t *)arg, PTHREAD_MUTEX_RECURSIVE_NP);
    }
#endif




    /* end mutex declared */

}
