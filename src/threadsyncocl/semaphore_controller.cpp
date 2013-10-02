
/* 						Titles														Authors 												Date
 * - Semaphore define														Chatsiri.rat                    01/10/2013
 */
#include "threadsyncocl/semaphore_controller.hpp"

namespace controller
{

    template<typename Mutex>
    bool mutex_buffer<Mutex>::init()
    {

        if(mx_ptr_vec.size() == 0) {
            mx_ptr ptr(new Mutex);
            Mutex   *m = ptr.get();

            if(!pthread_mutex_init(&m->mutex_t, NULL)) {
                mx_ptr_vec.push_back(ptr);
                return true;
            }
        } else {
            mx_ptr_vec.clear();
            mx_ptr ptr(new Mutex);
            mx_ptr_vec.push_back(ptr);
            return true;
        }

        return false;
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::lock_request()
    {
        boost::shared_ptr<Mutex> mx = mx_ptr_vec.back();
        Mutex *m = mx.get();

        if(!pthread_mutex_lock(&m->mutex_t)) {
            return true;
        }

        return false;
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::unlock_request()
    {
        boost::shared_ptr<Mutex> mx = mx_ptr_vec.back();
        Mutex *m = mx.get();

        if(!pthread_mutex_unlock(&m->mutex_t)) {
            return true;
        }

        return false;
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::try_lock()
    {


    }

    template<typename Mutex>
    boost::shared_ptr<Mutex> mutex_buffer<Mutex>::processes()
    {
        return mx_ptr_vec.back();
    }

    template<typename Mutex>
    bool mutex_buffer<Mutex>::destruction()
    {

        boost::shared_ptr<Mutex> mx = mx_ptr_vec.back();
        Mutex *m = mx.get();

        if(!pthread_mutex_destroy(&m->mutex_t)) {
            return true;
        }

        return false;
    }


    template class mutex_buffer<Mutex>;

}
