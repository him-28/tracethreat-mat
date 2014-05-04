#ifndef CONTROLLER_POSIXTHREAD_FACTORY_HPP
#define CONTROLLER_POSIXTHREAD_FACTORY_HPP

#include "threadconcurrency/thread.hpp"

namespace controller{

class posixthread_factory : public Thread_factory
{

    public:

        /**
         * POSIX Thread scheduler policies
         */
        enum POLICY {
            OTHER,
            FIFO,
            ROUND_ROBIN
        };

        /**
         * POSIX Thread scheduler relative priorities,
         *
         * Absolute priority is determined by scheduler policy and OS. This
         * enumeration specifies relative priorities such that one can specify a
         * priority withing a giving scheduler policy without knowing the absolute
         * value of the priority.
         */
        enum PRIORITY {
            LOWEST = 0,
            LOWER = 1,
            LOW = 2,
            NORMAL = 3,
            HIGH = 4,
            HIGHER = 5,
            HIGHEST = 6,
            INCREMENT = 7,
            DECREMENT = 8
        };

        /**
         * Posix thread (pthread) factory.  All threads created by a factory are reference-counted
         * via boost::shared_ptr and boost::weak_ptr.  The factory guarantees that threads and
         * the Runnable tasks they host will be properly cleaned up once the last strong reference
         * to both is given up.
         *
         * Threads are created with the specified policy, priority, stack-size and detachable-mode
         * detached means the thread is free-running and will release all system resources the
         * when it completes.  A detachable thread is not joinable.  The join method
         * of a detachable thread will return immediately with no error.
         *
         * By default threads are not joinable.
         */

        posixthread_factory(POLICY policy=ROUND_ROBIN, PRIORITY priority=NORMAL, int stackSize=1, bool detached=true);

        // From ThreadFactory;
        boost::shared_ptr<Thread> new_thread(boost::shared_ptr<Runnable> runnable) const;

        // From ThreadFactory;
        Thread::id_t get_current_thread_id() const;

        /**
         * Gets stack size for created threads
         *
         * @return int size in megabytes
         */
        virtual int getStackSize() const;

        /**
         * Sets stack size for created threads
         *
         * @param value size in megabytes
         */
        virtual void setStackSize(int value);

        /**
         * Gets priority relative to current policy
         */
        virtual PRIORITY getPriority() const;

        /**
         * Sets priority relative to current policy
         */
        virtual void setPriority(PRIORITY priority);

        /**
         * Sets detached mode of threads
         */
        virtual void setDetached(bool detached);

        /**
         * Gets current detached mode
         */
        virtual bool isDetached() const;

    private:
        class Impl;
        boost::shared_ptr<Impl> impl_;
};


}


#endif
