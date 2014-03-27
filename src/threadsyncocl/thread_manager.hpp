#ifndef CONTROLLER__THREAD_CONTROLLER_HPP_
#define CONTROLLER__THREAD_CONTROLLER_HPP_

//standard lib
#include <iostream>
#include "pthread.h"

// 3rd
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

// internal
#include "threadsyncocl/buffer_sync.hpp"
#include "threadsyncocl/semaphore_controller.hpp"
#include "threadsyncocl/thread_barrier_controller.hpp"

//
#include "utils/logger/clutil_logger.hpp"

namespace controller
{


    class runnable
    {
        public:
            virtual ~runnable() { }
            virtual void run() = 0;


            virtual boost::shared_ptr<thread> thread_() {
                return thread_.lock();
            }

            virtual void thread(boost::shared_ptr<thread> value) {
                thread_ = value;
            }

        private:
            boost::weak_ptr<thread> thread_;

    };


    // Common thread
    //template<typename BufferSync>
    class thread
    {

        public:
            //thread(boost::shared_ptr<runnable> run, bool detached = false);
            //thread(bool detached = false);
            //thread();

            //void start();
            //void *join();
#if  USE_BOOST_THREAD
            typedef boost::thread::id id_t;

            static inline bool is_current(id_t t) {
                return t == boost::this_thread::get_id();
            }
            static inline id_t get_current() {
                return boost::this_thread::get_id();
            }

#elif USE_STD_THREAD
            typedef std::thread::id id_t;

            static inline bool is_current(id_t t) {
                return t == std::this_thread::get_id();
            }
            static inline id_t get_current() {
                return std::this_thread::get_id();
            }
#else
            typedef pthread_t id_t;

            static bool is_current(id_t t) {
                return pthread_equal(pthread_self(), t);
            }
            static inline id_t get_current() {
                return pthread_self();
            }
#endif

            virtual ~Thread() { };

            virtual void start() = 0;

            virtual void join() = 0;

            virtual id_t getId() = 0;


            virtual boost::shared_ptr<runnable> runnable() const {
                return runnable_;
            }
        protected:

            virtual boost::shared_ptr<runnable> runnable(boost::shared_ptr<runnable>  value) const {
                return runnable_ = value;
            }

        private:
            boost::shared_ptr<runnable> runnable_;

            const thread& operator=(const thread&);
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

    //Thread factory
    template<typename BufferSync>
    class thread_factory
    {
        publc:
            virtual ~thread_factory() { }
            virtual boost::shared_ptr<thread<BufferSync> >
            new_thread(boost::shared_ptr<runnable>  runnable) const = 0;
            static const   thread::id_t   unknow_thread_id;
            virtual  thread::id_t  get_current_thread_id() const = 0;

    }

    // Thread Manager
    template<typename BufferSync>
    class thread_manager
    {

        public:
            class task;
            typedef boost::function<void(boost::shared_ptr<runnable>)> expire_callback;
            //thread(boost::shared_ptr<runnable> run, bool detached = false);
            thread_manager(bool detached = false);
            //thread();

            virtual void start() = 0;
            virtual void stop()  = 0;
            virtual void *join() = 0;

            enum STATE {
                UNINITIALIZED,
                STARTING,
                STARTED,
                JOINING,
                STOPPING,
                STOPPED
            };

            virtual STATE state() const = 0;

            virtual boost::shared_ptr<thread_factory>  thread_factory_sptr() const = 0;

            virtual void thread_factory(boost::shared_ptr<thread_factory>  value) = 0;

            virtual void add_worker(size_t value = 1 ) = 0;

            virtual void remove_worker(size_t value = 1) = 0;


            /*idle work count*/
            virtual size_t idle_worker_count() const = 0;


            /*current number of total worker */
            virtual size_t worker_count() const = 0;

            /*current number of pending tasks */
            virtual size_t pending_task_count() const = 0;

            /*cureent number of panding and executing tasks */
            virtual size_t total_task_count() const = 0;

            /*Maximum pending task count */
            virtual size_t pending_task_count_max() const = 0;

            /*Number of tasks which have been expired without being run */
            virtual size_t expired_task_count() = 0;

            /**
            * @brief Adds task to executed.
            *
            * @param task  task to queue for execution
            * @param timeout  0  : Wait forever to queue task
            *                 -1 : Return immediately if pending task count exceeds specified max
            * @param expiration  when non zero, the number of milliseconds the task is valid to be return; if
            * exceeded, the task will be dropped off the queue and not run.
            */
            virtual void add(boost::shared_ptr<runnable> task,
                    int64_t timeout=0LL,
                    int64_t expiration=0LL) = 0;


            /**
               * Removes a pending task
               */
            virtual void remove(boost::shared_ptr<Runnable> task) = 0;

            /**
             * Remove the next pending task which would be run.
             *
             * @return the task removed.
             */
            virtual boost::shared_ptr<Runnable> remove_next_pending() = 0;

            /**
             * Remove tasks from front of task queue that have expired.
             */
            virtual void remove_expired_tasks() = 0;

            /**
             * Set a callback to be called when a task is expired and not run.
             *
             * @param expireCallback a function called with the shared_ptr<Runnable> for
             * the expired task.
             */
            virtual void set_expire_callback(expire_callback expire_callback_) = 0;

            static boost::shared_ptr<thread_manager>  new_thread_manager();

            /**
             * Creates a simple thread manager the uses count number of worker threads and has
             * a pendingTaskCountMax maximum pending tasks. The default, 0, specified no limit
             * on pending tasks
             */
            static boost::shared_ptr<thread_manager>
            new_simple_thread_manager(size_t count=4, size_t pending_task_count_max=0);

            class task;

            class worker;

            class impl;


            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* THREAD_CONTROLLER_HPP */
