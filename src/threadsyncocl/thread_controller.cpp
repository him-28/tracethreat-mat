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
 * - Integrated thread to multifile scanning system.              R.Chatsiri          25/03/2014
 */


#include "threadsyncocl/thread_controller.hpp"
#include "utils/file_calculate.hpp"

namespace controller
{
    /*
    template<typename BufferSync>
    void thread_controller<BufferSync>::get_data()
    {
    //	logger->write_info("thread_controller, get_data()");
    }
    */
		
    template<typename BufferSync>
    thread_controller<BufferSync>::thread_controller()
    {
    }
		
		
    // Explicitly instance
    template class thread_controller<int>;
    template class thread_controller<BufferSync<char> >;
		

    /* todo: For testing with runnable, Subclass extend to runnable.
        template<typename BufferSync>
        thread<BufferSync>::thread(boost::shared_ptr<runnable> r, bool detached)
            : runnable_(r), detached_(detached)
        {
            if(!runnable_.get()) {
                logger->write_info(" Thread is detached ");
                exit(-1);
            }
        }
    */

		
    template<typename BufferSync>
    thread<BufferSync>::thread(bool detached)
        : runnable_(boost::shared_ptr<runnable>()), detached_(detached)
    {
        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();
        logger->write_info("Thread, Thread init/ not start ", h_util::format_type::type_header);
    }
		

    
        template<typename BufferSync>
        thread<BufferSync>::thread()
            : runnable_(boost::shared_ptr<runnable>())
        {
            //logger
            logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
            logger = logger_ptr->get();
            logger->write_info("Thread, Thread init/ not start ", h_util::format_type::type_header);
        }
    
		
    template<typename BufferSync>
    void *thread<BufferSync>::start_thread_runnable(void *p_void)
    {
        thread<BufferSync> *runnable_thread_ptr = static_cast<thread<BufferSync>* >(p_void);
        runnable_thread_ptr->result = runnable_thread_ptr->runnable_->run();
        //logger->write_info("Thread, start_thread_runnable"); // todo: Static cannot run with static_cast
        return runnable_thread_ptr->result;
    }

    template<typename BufferSync>
    void *thread<BufferSync>::start_thread(void *p_void)
    {
        thread<BufferSync> *thread_ptr = static_cast<thread<BufferSync>* >(p_void);
        thread_ptr->result = thread_ptr->run();
        //logger->write_info("Thread, start_thread"); // todo: Staitc cannot run with static_cast
        return thread_ptr->result;
    }

    template<typename BufferSync>
    void thread<BufferSync>::start()
    {
        logger->write_info("Thread, start() ");
        int status = pthread_attr_init(&thread_buffer_attr);

        if(status) {
            logger->write_info(" Attribute init fail ");
        }

        status = pthread_attr_setscope(&thread_buffer_attr, PTHREAD_SCOPE_SYSTEM);

        if(status) {
            logger->write_info(" setscope fail ");
        }

        if(!detached_) {
            if(!runnable_.get()) {
                status = pthread_create(&thread_buffer_id,
                        &thread_buffer_attr,
                        thread<BufferSync>::start_thread,
                        (void *)this);

                logger->write_info("Thread, ID : "); // add argument

                if(status) {
                    logger->write_info(" Cannot create start_thread ");
                }
            } else {
                status = pthread_create(&thread_buffer_id,
                        &thread_buffer_attr,
                        thread<BufferSync>::start_thread_runnable,
                        (void *)this);
                logger->write_info("Thread, ID : "); // add argument

                if(status) {
                    logger->write_info(" Cannot create start_thread_runnable ");
                }

            }
        }

        status = pthread_attr_destroy(&thread_buffer_attr);

        if(status) {
            logger->write_info(" cannot destroy ");
        }
    }

    template<typename BufferSync>
    void *thread<BufferSync>::join()
    {
        int status = pthread_join(thread_buffer_id, NULL);

        if(status) {
            logger->write_info(" Cannot join thread_buffer_id ");
        }
    }

    template<typename BufferSync>
    void  thread<BufferSync>::set_completed()
    {
        // join completed
    }


    //-- Communication thread buffer
    template<typename BufferSync>
    void *comm_thread_buffer<BufferSync>::run()
    {
        logger->write_info("-- To critical section --");
        mutex_buff->lock_request();
        buffer_sync_->set_buffer(5, "file_name");
        mutex_buff->unlock_request();
        logger->write_info("-- End of critical section --");
    }

    // Explicitly instance
    template class thread<BufferSync<buffer_kernel> >;
    template class thread<int>;
    template class comm_thread_buffer<BufferSync<buffer_kernel> >;
		
}


