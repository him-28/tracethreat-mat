
#include "threadconcurrency/thread_manager.hpp"
#include "threadconcurrency/monitor_controller.hpp"
#include "threadconcurrency/mutex_controller.hpp"
#include "threadconcurrency/util_thread.hpp"

#include <queue>

namespace controller
{

    //thread_manager
    using boost::shared_ptr;
    using boost::dynamic_pointer_cast;
		namespace exceptions = hnmav_exception::controller;
    /**
     * thread_manager class
     *
     * This class manages a pool of threads. It uses a thread_factory to create
     * threads.  It never actually creates or destroys worker threads, rather
     * it maintains statistics on number of idle threads, number of active threads,
     * task backlog, and average wait and service times.
     *
     * @version $Id:$
     */
    class thread_manager::impl : public thread_manager
    {

        public:
            impl() :
                worker_count_(0),
                worker_max_count_(0),
                idle_count_(0),
                pending_task_count_max_(0),
                expired_count_(0),
                state_(thread_manager::UNINITIALIZED),
                monitor_(&mutex_),
                max_monitor_(&mutex_) {}

            ~impl() {
                stop();
            }

            void start();

            void stop() {
                stop_impl(false);
            }

            void join() {
                stop_impl(true);
            }

            thread_manager::STATE state() const {
                return state_;
            }

            boost::shared_ptr<Thread_factory> thread_factory() const {
                synchronized s(monitor_);
                return thread_factory_;
            }

            void thread_factory(boost::shared_ptr<Thread_factory> value) {
                synchronized s(monitor_);
                thread_factory_ = value;
            }

            void add_worker(size_t value);

            void remove_worker(size_t value);

            size_t idle_worker_count() const {
                return idle_count_;
            }

            size_t worker_count() const {
                synchronized s(monitor_);
                return worker_count_;
            }

            size_t pending_task_count() const {
                synchronized s(monitor_);
                return tasks_.size();
            }

            size_t total_task_count() const {
                synchronized s(monitor_);
                return tasks_.size() + worker_count_ - idle_count_;
            }

            size_t pending_task_count_max() const {
                synchronized s(monitor_);
                return pending_task_count_max_;
            }

            size_t expired_task_count() {
                synchronized s(monitor_);
                size_t result = expired_count_;
                expired_count_ = 0;
                return result;
            }

            void pending_task_count_max(const size_t value) {
                synchronized s(monitor_);
                pending_task_count_max_ = value;
            }

            bool can_sleep();

            void add(boost::shared_ptr<Runnable> value, int64_t timeout, int64_t expiration);

            void remove(boost::shared_ptr<Runnable> task);

            boost::shared_ptr<Runnable> remove_next_pending();

            void remove_expired_tasks();

            void set_expire_callback(expire_callback expire_call_back);

        private:
            void stop_impl(bool join);

            size_t worker_count_;
            size_t worker_max_count_;
            size_t idle_count_;
            size_t pending_task_count_max_;
            size_t expired_count_;
            expire_callback expire_callback_;

            thread_manager::STATE state_;
            boost::shared_ptr<Thread_factory> thread_factory_;


            friend class thread_manager::task;
            std::queue<boost::shared_ptr<task> > tasks_;
            mutex_controller mutex_;
            monitor_controller monitor_;
            monitor_controller max_monitor_;
            monitor_controller worker_monitor_;

            friend class thread_manager::worker;
            std::set<boost::shared_ptr<Thread> > workers_;
            std::set<boost::shared_ptr<Thread> > dead_workers_;
            std::map<const Thread::id_t, boost::shared_ptr<Thread> > idMap_;
    };

    class thread_manager::task : public Runnable
    {

        public:
            enum STATE {
                WAITING,
                EXECUTING,
                CANCELLED,
                COMPLETE
            };

            task(boost::shared_ptr<Runnable> runnable, int64_t expiration=0LL)  :
                runnable_(runnable),
                state_(WAITING),
                expire_time_(expiration != 0LL ?
                        util_thread::current_time() + expiration : 0LL) {}

            ~task() {}

            void run() {
                if (state_ == EXECUTING) {
                    runnable_->run();
                    state_ = COMPLETE;
                }
            }

            boost::shared_ptr<Runnable> get_runnable() {
                return runnable_;
            }

            int64_t get_expire_time() const {
                return expire_time_;
            }

        private:
            boost::shared_ptr<Runnable> runnable_;
            friend class thread_manager::worker;
            STATE state_;
            int64_t expire_time_;
    };

    class thread_manager::worker: public Runnable
    {
            enum STATE {
                UNINITIALIZED,
                STARTING,
                STARTED,
                STOPPING,
                STOPPED
            };

        public:
            worker(thread_manager::impl *manager) :
                manager_(manager),
                state_(UNINITIALIZED),
                idle_(false) {}

            ~worker() {}

        private:
            bool is_active() const {
                return
                        (manager_->worker_count_ <= manager_->worker_max_count_) ||
                        (manager_->state_ == JOINING && !manager_->tasks_.empty());
            }

        public:
            /**
             * worker entry point
             *
             * As long as worker thread is running, pull tasks off the task queue and
             * execute.
             */
            void run() {
                bool active = false;
                bool notifyManager = false;

                /**
                * Increment worker semaphore and notify manager if worker count reached
                * desired max
                *
                * Note: We have to release the monitor and acquire the workermonitor_controller
                * since that is what the manager blocks on for worker add/remove
                */
                {
                    synchronized s(manager_->monitor_);
                    active = manager_->worker_count_ <
                            manager_->worker_max_count_;

                    if (active) {
                        manager_->worker_count_++;
                        notifyManager = manager_->worker_count_ ==
                                manager_->worker_max_count_;
                    }
                }

                if (notifyManager) {
                    synchronized s(manager_->worker_monitor_);
                    manager_->worker_monitor_.notify();
                    notifyManager = false;
                }

                while (active) {
                    boost::shared_ptr<thread_manager::task> task;

                    /**
                    * While holding manager monitor block for non-empty task queue (Also
                    * check that the thread hasn't been requested to stop). Once the queue
                    * is non-empty, dequeue a task, release monitor, and execute. If the
                    * worker max count has been decremented such that we exceed it, mark
                    * ourself inactive, decrement the worker count and notify the manager
                    * (technically we're notifying the next blocked thread but eventually
                    * the manager will see it.
                    */
                    {
                        guard g(manager_->mutex_);
                        active = is_active();

                        while (active && manager_->tasks_.empty()) {
                            manager_->idle_count_++;
                            idle_ = true;
                            manager_->monitor_.wait();
                            active = is_active();
                            idle_ = false;
                            manager_->idle_count_--;
                        }

                        if (active) {
                            manager_->remove_expired_tasks();

                            if (!manager_->tasks_.empty()) {
                                task = manager_->tasks_.front();
                                manager_->tasks_.pop();

                                if (task->state_ ==
                                        thread_manager::task::WAITING) {
                                    task->state_ =
                                            thread_manager::task::EXECUTING;
                                }

                                /* If we have a pending task max and we just dropped below it, wakeup any
                                                                 thread that might be blocked on add. */
                                if (manager_->pending_task_count_max_ != 0 &&
                                        manager_->tasks_.size() <=
                                        manager_->pending_task_count_max_ - 1) {
                                    manager_->max_monitor_.notify();
                                }
                            }
                        } else {
                            idle_ = true;
                            manager_->worker_count_--;
                            notifyManager = (manager_->worker_count_ ==
                                    manager_->worker_max_count_);
                        }
                    }

                    if (task) {
                        if (task->state_ == thread_manager::task::EXECUTING) {
                            try {
                                task->run();
                            } catch(...) {
                                // XXX need to log this
                            }
                        }
                    }
                }

                {
                    synchronized s(manager_->worker_monitor_);
                    manager_->dead_workers_.insert(this->thread());

                    if (notifyManager) {
                        manager_->worker_monitor_.notify();
                    }
                }

                return;
            }

        private:
            thread_manager::impl *manager_;
            friend class thread_manager::impl;
            STATE state_;
            bool idle_;
    };


    void thread_manager::impl::add_worker(size_t value)
    {
        std::set<boost::shared_ptr<Thread> > new_threads;

        for (size_t ix = 0; ix < value; ix++) {
            boost::shared_ptr<thread_manager::worker> worker =
                    boost::shared_ptr<thread_manager::worker>(
                            new thread_manager::worker(this));

            new_threads.insert(thread_factory_->new_thread(worker));
        }

        {
            synchronized s(monitor_);
            worker_max_count_ += value;
            workers_.insert(new_threads.begin(), new_threads.end());
        }

        for (std::set<boost::shared_ptr<Thread> >::iterator ix = new_threads.begin();
                ix != new_threads.end();
                ix++) {
            boost::shared_ptr<thread_manager::worker> worker =
                    dynamic_pointer_cast<thread_manager::worker,
                    Runnable>((*ix)->runnable());
            worker->state_ = thread_manager::worker::STARTING;
            (*ix)->start();
            idMap_.insert(std::pair<const Thread::id_t, boost::shared_ptr<Thread> >
                    ((*ix)->get_id(), *ix));
        }

        {
            synchronized s(worker_monitor_);

            while (worker_count_ != worker_max_count_) {
                worker_monitor_.wait();
            }
        }
    }

    void thread_manager::impl::start()
    {

        if (state_ == thread_manager::STOPPED) {
            return;
        }

        {
            synchronized s(monitor_);

            if (state_ == thread_manager::UNINITIALIZED) {
                if (!thread_factory_) {
                    throw exceptions::invalid_argument_exception();
                }

                state_ = thread_manager::STARTED;
                monitor_.notify_all();
            }

            while (state_ == STARTING) {
                monitor_.wait();
            }
        }
    }

    void thread_manager::impl::stop_impl(bool join)
    {
        bool do_stop = false;

        if (state_ == thread_manager::STOPPED) {
            return;
        }

        {
            synchronized s(monitor_);

            if (state_ != thread_manager::STOPPING &&
                    state_ != thread_manager::JOINING &&
                    state_ != thread_manager::STOPPED) {
                do_stop = true;
                state_ = join ? thread_manager::JOINING : thread_manager::STOPPING;
            }
        }

        if (do_stop) {
            remove_worker(worker_count_);
        }

        // XXX
        // should be able to block here for transition to STOPPED since we're no
        // using shared_ptrs

        {
            synchronized s(monitor_);
            state_ = thread_manager::STOPPED;
        }

    }

    void thread_manager::impl::remove_worker(size_t value)
    {
        std::set<shared_ptr<Thread> > removed_threads;
        {
            synchronized s(monitor_);

            if (value > worker_max_count_) {
                throw exceptions::invalid_argument_exception();
            }

            worker_max_count_ -= value;

            if (idle_count_ < value) {
                for (size_t ix = 0; ix < idle_count_; ix++) {
                    monitor_.notify();
                }
            } else {
                monitor_.notify_all();
            }
        }

        {
            synchronized s(worker_monitor_);

            while (worker_count_ != worker_max_count_) {
                worker_monitor_.wait();
            }

            for (std::set<boost::shared_ptr<Thread> >::iterator ix = dead_workers_.begin();
                    ix != dead_workers_.end();
                    ix++) {
                idMap_.erase((*ix)->get_id());
                workers_.erase(*ix);
            }

            dead_workers_.clear();
        }
    }

    bool thread_manager::impl::can_sleep()
    {
        const Thread::id_t id = thread_factory_->get_current_thread_id();
        return idMap_.find(id) == idMap_.end();
    }

    void thread_manager::impl::add(boost::shared_ptr<Runnable> value,
            int64_t timeout,
            int64_t expiration)
    {
        guard g(mutex_, timeout);

        if (!g) {
            throw exceptions::timed_out_exception();
        }

        if (state_ != thread_manager::STARTED) {
            throw exceptions::illegal_state_exception("thread_manager::impl::add ThreadManager "
                    "not started");
        }

        remove_expired_tasks();

        if (pending_task_count_max_ > 0 && (tasks_.size() >= pending_task_count_max_)) {
            if (can_sleep() && timeout >= 0) {
                while (pending_task_count_max_ > 0 && tasks_.size() >= pending_task_count_max_) {
                    // This is thread safe because the mutex is shared between monitors.
                    max_monitor_.wait(timeout);
                }
            } else {
                throw exceptions::too_many_pending_tasks_exception();
            }
        }

        tasks_.push(boost::shared_ptr<thread_manager::task>(new thread_manager::task(value, expiration)));

        // If idle thread is available notify it, otherwise all worker threads are
        // running and will get around to this task in time.
        if (idle_count_ > 0) {
            monitor_.notify();
        }
    }

    void thread_manager::impl::remove(shared_ptr<Runnable> task)
    {
        (void) task;
        synchronized s(monitor_);

        if (state_ != thread_manager::STARTED) {
            throw exceptions::illegal_state_exception("thread_manager::impl::remove ThreadManager not "
                    "started");
        }
    }

    boost::shared_ptr<Runnable> thread_manager::impl::remove_next_pending()
    {
        guard g(mutex_);

        if (state_ != thread_manager::STARTED) {
            throw exceptions::illegal_state_exception("thread_manager::impl::removeNextPending "
                    "thread_manager not started");
        }

        if (tasks_.empty()) {
            return boost::shared_ptr<Runnable>();
        }

        boost::shared_ptr<thread_manager::task> task = tasks_.front();
        tasks_.pop();

        return task->get_runnable();
    }

    void thread_manager::impl::remove_expired_tasks()
    {
        int64_t now = 0LL; // we won't ask for the time untile we need it

        // note that this loop breaks at the first non-expiring task
        while (!tasks_.empty()) {
            boost::shared_ptr<thread_manager::task> task = tasks_.front();

            if (task->get_expire_time() == 0LL) {
                break;
            }

            if (now == 0LL) {
                now = util_thread::current_time();
            }

            if (task->get_expire_time() > now) {
                break;
            }

            if (expire_callback_) {
                expire_callback_(task->get_runnable());
            }

            tasks_.pop();
            expired_count_++;
        }
    }


    void thread_manager::impl::set_expire_callback(expire_callback expire_callback)
    {
        expire_callback_ = expire_callback;
    }

    class simplethread_manager : public thread_manager::impl
    {

        public:
            simplethread_manager(size_t worker_count=4, size_t pending_task_count_max=0) :
                worker_count_(worker_count),
                pending_task_count_max_(pending_task_count_max),
                firstTime_(true) {
            }

            void start() {
                thread_manager::impl::pending_task_count_max(pending_task_count_max_);
                thread_manager::impl::start();
                add_worker(worker_count_);
            }

        private:
            const size_t worker_count_;
            const size_t pending_task_count_max_;
            bool firstTime_;
            monitor_controller monitor_;
    };


    boost::shared_ptr<thread_manager> thread_manager::new_thread_manager()
    {
        return boost::shared_ptr<thread_manager>(new thread_manager::impl());
    }

    boost::shared_ptr<thread_manager> thread_manager
    ::new_simple_thread_manager(size_t count, size_t pending_task_count_max)
    {
        return boost::shared_ptr<thread_manager>(new simplethread_manager(count, pending_task_count_max));
    }

}


