#ifndef THREAD_SYNC_HPP
#define THREAD_SYNC_HPP

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
 *  - Sync controller between Thread and OCL                       R.Chatsir      08/10/2013
 *  - Get file to thread multithread                               R.Chatsiri     08/10/2013
 *  - Add sync buffer to OCL                                       R.Chatsiri     25/03/2014
 */

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

    using namespace util;
    namespace h_util = hnmav_util;

    template<typename BufferSync>
    class ibuffer_sync
    {
        public:
            virtual boost::shared_ptr<BufferSync>& buffer_ocl() = 0;
            virtual std::string path()= 0;
            virtual ibuffer_sync& sync_processes() = 0;
    };

    template<typename BufferSync>
    class thread_sync : public ibuffer_sync<BufferSync>
    {

        private:
            boost::shared_ptr<BufferSync> buffer_sync;

            //file processes
            file_calculate<Extension> *file_cal;
            std::list<std::string> *list_files;

            //thread controller
            typedef BufferSync      buffer_sync_type;
            typedef buffer_kernel::size_int size_type;
            typedef boost::shared_ptr<comm_thread_buffer<buffer_sync_type> > thread_ptr;

            std::string  *file_path;
            thread_ptr   *thread_array_ptr;
            buffer_sync_type *buff_sync_internal;

            size_type thread_id;

            std::vector<thread_ptr> thread_ptr_vec;
            // pointer handler vector of shared_ptr of threads.
            std::vector<thread_ptr> *thread_pv_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;


        public:

            thread_sync();

            boost::shared_ptr<BufferSync>& buffer_ocl();

            std::string path();
            void set_path(const char *path);

            //processe sync and controller
            std::vector<boost::shared_ptr<comm_thread_buffer<BufferSync> > >&   sync_init();
            bool sync_buffer();
            bool sync_check_kernel();

            boost::tuple<buffer_kernel::size_int> get_thread_info();

            ibuffer_sync<BufferSync>& sync_processes();

    };

}

#endif /* THREAD_THREAD_SYNC_HPP */
