#ifndef  CONTROLLER_THREAD_BARRIER_CONTROLLER_HPP
#define  CONTROLLER_THREAD_BARRIER_CONTROLLER_HPP

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

/*  Titles			                                 Authors	          Date
 * - Thread barrier controller support OCL    R.Chatsiri      30/03/2014
 */

#include "pthread.h"
#include "threadsyncocl/buffer_sync.hpp"

//
#include "utils/logger/clutil_logger.hpp"

namespace controller
{

		namespace h_util = hnmav_util;

    template<typename BarrierThread>
    class barrier_controller;

    struct barrier_thread {
        typedef pthread_barrier_t barrier_t;
        typedef struct buffer_kernel buffer;
        typedef pthread_condattr_t  condattr_t;
        typedef pthread_cond_t      cond_t;
        //typedef pthread_cond_   cond_wait_t;
    };


    template<typename BarrierThread>
    class barrier
    {
        public:
            virtual bool barrier_init(barrier_controller<BarrierThread> *bc) = 0;
            virtual bool barrier_wait(barrier_controller<BarrierThread> *bc) = 0;
            virtual bool barrier_condition(barrier_controller<BarrierThread> *bc) = 0;

    };

    template<typename BarrierThread>
    class barrier_controller : public barrier<BarrierThread>
    {

        protected:

            bool barrier_init(barrier_controller<BarrierThread> *b) {
                b->barrier_init();
            }

            bool barrier_wait(barrier_controller<BarrierThread> *b) {
                b->barrier_wait();
            }

            bool barrier_condition(barrier_controller<BarrierThread> *b) {
                b->barrier_condition();
            }
        public:
            virtual bool barrier_init() = 0;
            virtual bool condition_init() = 0;
            virtual bool barrier_wait() = 0;
            virtual bool barrier_condition() = 0;
            virtual bool destroy() = 0;
            //virtual BarrierThread& get_barrier() = 0;
						//logger
					  boost::shared_ptr<h_util::clutil_logging<std::string, int> > * logger_ptr;
						h_util::clutil_logging<std::string, int>   * logger;

    };


    template<typename BarrierThread>
    class barrier_buffer : public barrier_controller<BarrierThread>
    {

        public:
            barrier_buffer(const typename
                    BarrierThread::buffer::thread_int thread_num)
                : thread_num_(thread_num) { }
            bool barrier_init();
            bool condition_init();
            bool barrier_wait();
            bool barrier_condition();
            bool destroy();
        private:
            typedef BarrierThread b_t;
            typename b_t::barrier_t p_barrier;
            typename b_t::condattr_t p_condattr;
            typename b_t::buffer::thread_int const thread_num_;
            typename b_t::cond_t p_cond;
            //b_t::cond_wait_t p_cond_wait;
            //logger
					  boost::shared_ptr<h_util::clutil_logging<std::string, int> > * logger_ptr;
						h_util::clutil_logging<std::string, int>   * logger;

    };

    template class barrier_buffer<barrier_thread>;
    template class barrier_controller<barrier_thread>;
    template class barrier<barrier_thread>;
}

#endif /*  CONTROLLER_THREAD_BARRIER_CONTROLLER_HPP */


