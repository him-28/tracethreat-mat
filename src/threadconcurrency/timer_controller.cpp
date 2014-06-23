#include "threadconcurrency/timer_controller.hpp"
#include "exception/system_exception.hpp"
#include "threadconcurrency/util_thread.hpp"

#include <assert.h>
#include <iostream>
#include <set>

namespace controller
{

    using boost::shared_ptr;

    namespace exceptions = hnmav_exception::controller;

    /**
     * timer_manager class
     *
     * @version $Id:$
     */
    class timer_manager::Task : public Runnable
    {

        public:
            enum STATE {
                WAITING,
                EXECUTING,
                CANCELLED,
                COMPLETE
            };

            Task(shared_ptr<Runnable> runnable) :
                runnable_(runnable),
                state_(WAITING) {}

            ~Task() {
            }

            void run() {
                if (state_ == EXECUTING) {
                    runnable_->run();
                    state_ = COMPLETE;
                }
            }

        private:
            shared_ptr<Runnable> runnable_;
            friend class timer_manager::Dispatcher;
            STATE state_;
    };

    class timer_manager::Dispatcher: public Runnable
    {

        public:
            Dispatcher(timer_manager *manager) :
                manager_(manager) {}

            ~Dispatcher() {}

            /**
             * Dispatcher entry point
             *
             * As long as dispatcher thread is running, pull tasks off the task taskMap_
             * and execute.
             */
            void run() {
                {
                    synchronized s(manager_->monitor_);

                    if (manager_->state_ == timer_manager::STARTING) {
                        manager_->state_ = timer_manager::STARTED;
                        manager_->monitor_.notify_all();
                    }
                }

                do {
                    std::set<shared_ptr<timer_manager::Task> > expiredTasks;
                    {
                        synchronized s(manager_->monitor_);
                        task_iterator expiredTaskEnd;
                        int64_t now = util_thread::current_time();

                        while (manager_->state_ == timer_manager::STARTED &&
                                (expiredTaskEnd = manager_->taskMap_.upper_bound(now)) == manager_->taskMap_.begin()) {
                            int64_t timeout = 0LL;

                            if (!manager_->taskMap_.empty()) {
                                timeout = manager_->taskMap_.begin()->first - now;
                            }

                            assert((timeout != 0 && manager_->taskCount_ > 0) || (timeout == 0 && manager_->taskCount_ == 0));

                            try {
                                manager_->monitor_.wait(timeout);
                            }catch(exceptions::timed_out_exception) { } 

                            //catch (TimedOutException&) {}

                            now = util_thread::current_time();
                        }

                        if (manager_->state_ == timer_manager::STARTED) {
                            for (task_iterator ix = manager_->taskMap_.begin(); ix != expiredTaskEnd; ix++) {
                                shared_ptr<timer_manager::Task> task = ix->second;
                                expiredTasks.insert(task);

                                if (task->state_ == timer_manager::Task::WAITING) {
                                    task->state_ = timer_manager::Task::EXECUTING;
                                }

                                manager_->taskCount_--;
                            }

                            manager_->taskMap_.erase(manager_->taskMap_.begin(), expiredTaskEnd);
                        }
                    }

                    for (std::set<shared_ptr<Task> >::iterator ix =  expiredTasks.begin(); ix != expiredTasks.end(); ix++) {
                        (*ix)->run();
                    }

                } while (manager_->state_ == timer_manager::STARTED);

                {
                    synchronized s(manager_->monitor_);

                    if (manager_->state_ == timer_manager::STOPPING) {
                        manager_->state_ = timer_manager::STOPPED;
                        manager_->monitor_.notify();
                    }
                }
                return;
            }

        private:
            timer_manager *manager_;
            friend class timer_manager;
    };

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4355) // 'this' used in base member initializer list
#endif

    timer_manager::timer_manager() :
        taskCount_(0),
        state_(timer_manager::UNINITIALIZED),
        dispatcher_(shared_ptr<Dispatcher>(new Dispatcher(this)))
    {
    }

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

    timer_manager::~timer_manager()
    {

        // If we haven't been explicitly stopped, do so now.  We don't need to grab
        // the monitor here, since stop already takes care of reentrancy.

        if (state_ != STOPPED) {
            try {
                stop();
            } catch(...) {
                throw;
                // uhoh
            }
        }
    }

    void timer_manager::start()
    {
        bool doStart = false;
        {
            synchronized s(monitor_);

            if (!threadFactory_) {
                throw exceptions::invalid_argument_exception();//InvalidArgumentException();
            }

            if (state_ == timer_manager::UNINITIALIZED) {
                state_ = timer_manager::STARTING;
                doStart = true;
            }
        }

        if (doStart) {
            dispatcherThread_ = threadFactory_->new_thread(dispatcher_);
            dispatcherThread_->start();
        }

        {
            synchronized s(monitor_);

            while (state_ == timer_manager::STARTING) {
                monitor_.wait();
            }

            assert(state_ != timer_manager::STARTING);
        }
    }

    void timer_manager::stop()
    {
        bool doStop = false;
        {
            synchronized s(monitor_);

            if (state_ == timer_manager::UNINITIALIZED) {
                state_ = timer_manager::STOPPED;
            } else if (state_ != STOPPING &&  state_ != STOPPED) {
                doStop = true;
                state_ = STOPPING;
                monitor_.notify_all();
            }

            while (state_ != STOPPED) {
                monitor_.wait();
            }
        }

        if (doStop) {
            // Clean up any outstanding tasks
            taskMap_.clear();

            // Remove dispatcher's reference to us.
            dispatcher_->manager_ = NULL;
        }
    }

    shared_ptr<const Thread_factory> timer_manager::threadFactory() const
    {
        synchronized s(monitor_);
        return threadFactory_;
    }

    void timer_manager::threadFactory(shared_ptr<const Thread_factory>  value)
    {
        synchronized s(monitor_);
        threadFactory_ = value;
    }

    size_t timer_manager::taskCount() const
    {
        return taskCount_;
    }

    void timer_manager::add(shared_ptr<Runnable> task, int64_t timeout)
    {
        int64_t now = util_thread::current_time();
        timeout += now;

        {
            synchronized s(monitor_);

            if (state_ != timer_manager::STARTED) {
                throw exceptions::illegal_state_exception();
//IllegalStateException();
            }

            // If the task map is empty, we will kick the dispatcher for sure. Otherwise, we kick him
            // if the expiration time is shorter than the current value. Need to test before we insert,
            // because the new task might insert at the front.
            bool notifyRequired = (taskCount_ == 0) ? true : timeout < taskMap_.begin()->first;

            taskCount_++;
            taskMap_.insert(std::pair<int64_t, shared_ptr<Task> >(timeout, shared_ptr<Task>(new Task(task))));

            // If the task map was empty, or if we have an expiration that is earlier
            // than any previously seen, kick the dispatcher so it can update its
            // timeout
            if (notifyRequired) {
                monitor_.notify();
            }
        }
    }

    void timer_manager::add(shared_ptr<Runnable> task, const struct  TIMESPEC& value)
    {

        int64_t expiration;
        util_thread::to_milliseconds(expiration, value);

        int64_t now = util_thread::current_time();

        if (expiration < now) {
            throw exceptions::invalid_argument_exception();
        }

        add(task, expiration - now);
    }

    void timer_manager::add(shared_ptr<Runnable> task, const struct timeval& value)
    {

        int64_t expiration;
        util_thread::to_milliseconds(expiration, value);

        int64_t now = util_thread::current_time();

        if (expiration < now) {
            throw  exceptions::invalid_argument_exception();
        }

        add(task, expiration - now);
    }

    void timer_manager::remove(shared_ptr<Runnable> task)
    {
        (void) task;
        synchronized s(monitor_);

        if (state_ != timer_manager::STARTED) {
            throw exceptions::illegal_state_exception();
 //IllegalStateException();
        }
    }

    timer_manager::STATE timer_manager::state() const
    {
        return state_;
    }

}  //controller

