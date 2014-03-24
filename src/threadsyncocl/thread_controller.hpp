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

    namespace h_util = hnmav_util;

    template<typename BufferSync>
    class thread_controller
    {
        public:
            thread_controller();
						
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
            //thread(boost::shared_ptr<runnable> run, bool detached = false);
            thread(bool detached = false);
						//thread();

            void start();
            void *join();
				#if  USE_BOOST_THREAD
					typedef boost::thread::id id_t;
					
					static inline bool is_current(id_t t) { return t == boost::this_thread::get_id(); }
					static inline id_t get_current(){ return boost::this_thread::get_id(); }

				#elif USE_STD_THREAD
					typedef std::thread::id id_t;		
					
					static inline bool is_current(id_t t) { return t == std::this_thread::get_id(); }
					static inline id_t get_current(){ return std::this_thread::get_id(); }
				#else
					typedef pthread_t id_t;	
					
					static bool is_current(id_t t){ return pthread_equal(pthread_self(), t); }
					static inline id_t get_current(){ return pthread_self(); }
				#endif
							
					
        private:
            bool detached_;

            thread(const thread&);

            boost::shared_ptr<runnable> runnable_;

            const thread& operator=(const thread&);

            void set_completed();

            void *run() { }

            static void *start_thread_runnable(void *p_void);
            static void *start_thread(void   *p_void);

            //BufferSync buffer_sync;
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
	          //logger
					  boost::shared_ptr<h_util::clutil_logging<std::string, int> > * logger_ptr;
						h_util::clutil_logging<std::string, int>   * logger;

    };


		//Thread factory
		template<typename BufferSync>
		class thread_factory{
			publc:
					virtual ~thread_factory(){ }
					virtual boost::shared_ptr<thread<BufferSync> > 
										new_thread(boost::shared_ptr<runnable>  runnable) const = 0;
					static const   thread::id_t   unknow_thread_id;
					virtual  thread::id_t  get_current_thread_id() const = 0;

		}

}


#endif /* THREAD_CONTROLLER_HPP */
