#ifndef CONTROLLER__SEMAPHORE_CONTROLLER_HPP_
#define CONTROLLER__SEMAPHORE_CONTROLLER_HPP_

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
 *
 */



#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <vector>

//
#include "utils/logger/clutil_logger.hpp"

namespace controller
{
    //work around with h_ prefix
    namespace h_util = hnmav_util;
    //Define type class
    class Mutex
    {
        public:
            pthread_mutex_t mutex_t;
    };

    // Bule print of mutex.
    template<typename SemaphoreProcesses>
    class semaphore
    {
        public:
            virtual bool lock_request()   = 0;
            virtual bool unlock_request() = 0;
            virtual boost::shared_ptr<SemaphoreProcesses> processes() = 0;
    };

    // Mutex
    template<typename Mutex>
    class mutex_buffer : public semaphore<Mutex>
    {
        public:
            bool init();
            bool lock_request();
            bool unlock_request();
            bool try_lock();
            boost::shared_ptr<Mutex> processes();
            bool destruction();
        private:
            typedef boost::shared_ptr<Mutex>  mx_ptr;
            //move sementics
            std::vector<boost::shared_ptr<Mutex> >   mx_ptr_vec;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

}

#endif /* CONTROLLER__SEMAPHORE_CONTROLLER_HPP_ */
