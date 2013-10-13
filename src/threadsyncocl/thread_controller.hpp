#ifndef THREAD_CONTROLLER_HPP_
#define THREAD_CONTROLLER_HPP_

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

    namespace h_util = hnmav_util;

    template<typename BufferSync>
    class thread_controller
    {
        public:
            thread_controller();
            void get_data();

        private:

    };

    // Runnable run threads
    class runnable
    {
        public:
            virtual void *run() = 0;
            virtual ~runnable() = 0;

    };

    //runnable::~runnable() {};


    // Common thread
    template<typename BufferSync>
    class thread
    {

        public:
            thread(boost::shared_ptr<runnable> run, bool detached = false);
            thread(bool detached = false);
            void start();
            void *join();

        private:
            bool detached_;

            thread(const thread&);

            boost::shared_ptr<runnable> runnable_;

            const thread& operator=(const thread&);

            void set_completed();

            void *run() { }

            static void *start_thread_runnable(void *p_void);
            static void *start_thread(void   *p_void);

            BufferSync buffer_sync;
            void *result;
            pthread_t thread_buffer_id;
            pthread_attr_t thread_buffer_attr;

            //logger
					  boost::shared_ptr<h_util::clutil_logging<std::string, int> > * logger_ptr;
						h_util::clutil_logging<std::string, int>   * logger;

    };

    // communication thread buffer
    template<typename BufferSync>
    class comm_thread_buffer : public thread<BufferSync>
    {
        public:
            comm_thread_buffer(typename buffer_kernel::size_int ID,
                    BufferSync *const  buffer_sync) :
                my_id(ID),
                buffer_sync_(buffer_sync) {
                mutex_buff = new mutex_buffer<Mutex>();
                mutex_buff->init();
            }
            void *run();
        private:
            typename buffer_kernel::size_int  my_id;
            BufferSync *buffer_sync_;
            mutex_buffer<Mutex> *mutex_buff;
    };


}


#endif /* THREAD_CONTROLLER_HPP */
