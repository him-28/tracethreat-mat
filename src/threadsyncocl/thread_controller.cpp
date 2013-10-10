#include "threadsyncocl/thread_controller.hpp"
#include "utils/file_calculate.hpp"

namespace controller
{

template<typename BufferSync>
void thread_controller<BufferSync>::get_data()
{
    std::cout<<"Thread controller get_data " <<std::endl;
}

template<typename BufferSync>
thread_controller<BufferSync>::thread_controller()
{

}

// Explicitly instance 
template class thread_controller<int>;
template class thread_controller<BufferSync<char> >;

template<typename BufferSync>
thread<BufferSync>::thread(boost::shared_ptr<runnable> r, bool detached)
    : runnable_(r), detached_(detached)
{
    if(!runnable_.get()) {
        std::cout<<" Thread is detached " <<std::endl;
        exit(-1);
    }
}

template<typename BufferSync>
thread<BufferSync>::thread(bool detached)
    : runnable_(boost::shared_ptr<runnable>()), detached_(detached) { }

template<typename BufferSync>
void *thread<BufferSync>::start_thread_runnable(void *p_void)
{
    thread<BufferSync> *runnable_thread_ptr = static_cast<thread<BufferSync>* >(p_void);
    runnable_thread_ptr->result = runnable_thread_ptr->runnable_->run();
    return runnable_thread_ptr->result;
}

template<typename BufferSync>
void *thread<BufferSync>::start_thread(void *p_void)
{
    thread<BufferSync> *thread_ptr = static_cast<thread<BufferSync>* >(p_void);
    thread_ptr->result = thread_ptr->run();
    return thread_ptr->result;
}

template<typename BufferSync>
void thread<BufferSync>::start()
{
    int status = pthread_attr_init(&thread_buffer_attr);

    if(status) {
        std::cout<<" attr init fail " <<std::endl;
    }

    status = pthread_attr_setscope(&thread_buffer_attr, PTHREAD_SCOPE_SYSTEM);

    if(status) {
        std::cout<<" setscope fail " <<std::endl;
    }

    if(!detached_) {
        if(!runnable_.get()) {
            status = pthread_create(&thread_buffer_id,
                    &thread_buffer_attr,
                    thread<BufferSync>::start_thread,
                    (void *)this);

            if(status) {
                std::cout<<" Cannot create start_thread " <<std::endl;
            }
        } else {
            status = pthread_create(&thread_buffer_id,
                    &thread_buffer_attr,
                    thread<BufferSync>::start_thread_runnable,
                    (void *)this);

            if(status) {
                std::cout<<" Cannot create start_thread_runnable " <<std::endl;
            }

        }
    }

    status = pthread_attr_destroy(&thread_buffer_attr);

    if(status) {
        std::cout<<" cannot destroy " <<std::endl;
    }	
}

template<typename BufferSync>
void *thread<BufferSync>::join()
{
    int status = pthread_join(thread_buffer_id, NULL);

    if(status) {
        std::cout<<" Cannot join thread_buffer_id " <<std::endl;
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
		mutex_buff->lock_request();
    buffer_sync_->set_buffer();
		mutex_buff->unlock_request();

}

// Explicitly instance 
template class thread<BufferSync<buffer_kernel> >;
template class thread<int>;
template class comm_thread_buffer<BufferSync<buffer_kernel> >;

}


