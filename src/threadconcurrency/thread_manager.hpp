#ifndef CONTROLLER_THREAD_CONTROLLER_HPP
#define CONTROLLER_THREAD_CONTROLLER_HPP

//standard lib
#include <iostream>
#include "pthread.h"

// 3rd
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

// internal
#include "threadconcurrency/thread.hpp"

//
#include "utils/logger/clutil_logger.hpp"

namespace controller
{
		class thread_manager;

    // Thread Manager
    class thread_manager
    {
				protected:
					 thread_manager(){ }
        public:
            class task;
            typedef boost::function<void(boost::shared_ptr<Runnable>)> expire_callback;

						virtual ~thread_manager(){ }

            virtual void start() = 0;
            virtual void stop()  = 0;
            virtual void join() = 0;

            enum STATE {
                UNINITIALIZED,
                STARTING,
                STARTED,
                JOINING,
                STOPPING,
                STOPPED
            };

            virtual STATE state() const = 0;

            virtual boost::shared_ptr<Thread_factory>  thread_factory() const = 0;

            virtual void thread_factory(boost::shared_ptr<Thread_factory>  value) = 0;

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
            virtual void add(boost::shared_ptr<Runnable> task,
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
            //boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            //h_util::clutil_logging<std::string, int>    *logger;

    };

           //boost::shared_ptr<thread_manager>  thread_manager::new_thread_manager();

            /**
             * Creates a simple thread manager the uses count number of worker threads and has
             * a pendingTaskCountMax maximum pending tasks. The default, 0, specified no limit
             * on pending tasks
             */
            //boost::shared_ptr<thread_manager>
            //::new_simple_thread_manager(size_t count=4, size_t pending_task_count_max=0);



}


#endif /* THREAD_CONTROLLER_HPP */
