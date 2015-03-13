/*
* Copyright 2014 Chatsiri Rattana.
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
 * - Semaphore define																						Chatsiri.rat      01/10/2013
 */
#include "threadsyncocl/semaphore_controller.hpp"

//#include "utils/base/util_thread.hpp"


namespace controller
{

    template<typename Mutex>
    bool mutex_buffer<Mutex>::init()
    {
        // logger
        logger_ptr = &utils::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();


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
        //logger->write_info(" Lock request ");
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
        //logger->write_info(" Unlock request ");
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
