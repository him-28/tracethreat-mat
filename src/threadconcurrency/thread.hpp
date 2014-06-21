#ifndef CONTROLLER_THREAD_HPP
#define CONTROLLER_THREAD_HPP

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <pthread.h>

namespace controller{
		class Thread;

    class Runnable
    {
        public:
            virtual ~Runnable() { }
            virtual void run() = 0;


            virtual boost::shared_ptr<Thread> thread() {
                return thread_.lock();
            }

            virtual void thread(boost::shared_ptr<Thread> value) {
                thread_ = value;
            }

        private:
            boost::weak_ptr<Thread> thread_;

    };


    // Common Thread
    //template<typename BufferSync>
    class Thread
    {

        public:
            //Thread(boost::shared_ptr<runnable> run, bool detached = false);
            //Thread(bool detached = false);
            //Thread();

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
                return std::this_Thread::get_id();
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

            virtual id_t get_id() = 0;


            virtual boost::shared_ptr<Runnable> runnable() const {
                return runnable_;
            }
        protected:

            virtual void runnable(boost::shared_ptr<Runnable>  value){
                runnable_ = value;
            }

        private:
            boost::shared_ptr<Runnable> runnable_;

            //const Thread& operator=(const Thread&);
            //logger
            //boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            //h_util::clutil_logging<std::string, int>    *logger;

    };

    //Thread factory
    class Thread_factory
    {
        public:
            virtual ~Thread_factory() { }
            virtual boost::shared_ptr<Thread>
            new_thread(boost::shared_ptr<Runnable>  runnable) const = 0;
            static const   Thread::id_t   unknow_thread_id;
            virtual  Thread::id_t  get_current_thread_id() const = 0;

    };


}

#endif

