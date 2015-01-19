#ifndef CONTROLLER__THREAD_CONTROLLER_HPP
#define CONTROLLER__THREAD_CONTROLLER_HPP

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

/*  Titles																													 Authors						Date
 * - Integrated thread to multifile scanning system.              R.Chatsiri          25/03/2014
 * - Add slot_ocl_thread for manage ocl and multithread scanning  R.Chatsiri          07/04/2014
 */

//standard lib
#include <iostream>
#include "pthread.h"

// 3rd
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

// internal
#include "taskconcurrency/buffer_sync.hpp"

#include "threadsyncocl/semaphore_controller.hpp"
#include "threadsyncocl/thread_barrier_controller.hpp"

//Logging
#include "utils/logger/clutil_logger.hpp"
#include "utils/base/system_code.hpp"

#include "ocl/cl_bootstrap.hpp"

#include "memory/signature_shm_base.hpp"
#include "memory/signature_shm_controller.hpp"

namespace filetypes
{

    template<typename MAPPED_FILE>
    class pe_file_controller;
}

namespace controller
{



    namespace h_util = hnmav_util;
    //using namespace filetypes;

    namespace dstr   = data_structure;
    namespace kernel_ocl = hnmav_kernel;
		using  memory::signature_shm;

    class thread_controller
    {
        public:
            virtual pthread_t& get_thread_id() = 0;
            //virtual pthread_t & get_thread_id() = 0;
    };

    // Runnable run threads
    class runnable
    {
        public:
            virtual void *run() = 0;
            virtual ~runnable() = 0;

    };



    //runnable::~runnable() {};


    /*old thread communicator */
    template<typename BufferSync>
    class thread
    {

        public:

            //thread(boost::shared_ptr<runnable> run, bool detached = false);
            thread(bool detached = false);
            //thread();

            void start();
            void *join();

            //protected:
            typedef pthread_t id_t;

            id_t  get_thread_id();

            inline bool is_current(id_t t);

            bool thread_cancel();

        private:

            bool detached_;

            thread(const thread&);

            boost::shared_ptr<runnable> runnable_;

            const thread& operator=(const thread&);

            void set_completed();

            virtual void *run() { }

            static void *start_thread_runnable(void *p_void);
            static void *start_thread(void   *p_void);

            //BufferSync buffer_sync;
            void *result;
            pthread_attr_t thread_buffer_attr;
            id_t  thread_buffer_id;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };



    // communication thread buffer
    template<typename BufferSync, typename MAPPED_FILE>
    class comm_thread_buffer : public thread<BufferSync>
    {
        public:
						typedef memory::signature_shm<struct memory::meta_sig, struct memory::meta_sig_mem>
							signature_shm_type;

            comm_thread_buffer(uint64_t ID,
                    BufferSync *const  buffer_sync,
                    mutex_buffer<Mutex> *mutex_buff,
                    signature_shm_type *sig_shm) :
                my_id(ID),
                buffer_sync_(buffer_sync),
                mutex_buff_(mutex_buff),
								sig_shm_(sig_shm){
                //Initial mutex after set buffer to buffer_sync_
                //mutex_buff = new mutex_buffer<Mutex>();
                //mutex_buff->init();
            }
            virtual void *run();
            ~comm_thread_buffer();
        private:
            //typename buffer_kernel::size_int  my_id;
            uint64_t  my_id;
            BufferSync *buffer_sync_;
            mutex_buffer<Mutex> *mutex_buff_;
						
						signature_shm_type * sig_shm_;
            //file md5 per thread.
            struct slot_ocl *s_ocl;
            struct data_ocl_process<MAPPED_FILE> *d_ocl_processes;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


    template<typename BufferSync, typename MAPPED_FILE>
    class slot_ocl_thread : public thread<BufferSync>
    {
        public:

            typedef kernel_ocl::cl_load_system<kernel_ocl::clutil_platform,
                    dstr::dstr_def::work_groupitems,
                    std::vector<boost::unordered_map<char, size_t> >,
                    dstr::actire_parallel<char,
                    size_t,
                    boost::unordered_map<char, size_t>,
                    std::vector<boost::unordered_map<char, size_t> > >
                    >	 load_ocl_system_type;

        public:
            slot_ocl_thread(typename slot_ocl_thread<BufferSync, MAPPED_FILE>::
                    load_ocl_system_type *load_ocl_system,
                    BufferSync *const  buffer_sync,
                    mutex_buffer<Mutex> *mutex_buff) :
                load_ocl_system_(load_ocl_system), // OCL command queue.
                buffer_sync_(buffer_sync), // Internal buffer of binary file.
                mutex_buff_(mutex_buff) {  // Mutex

            }
            virtual void *run();

            bool set_tid_task(std::vector<pthread_t> p_tid_task_vec) {
                this->p_tid_task_vec = p_tid_task_vec;
            }

            ~slot_ocl_thread();

        private:
            //typename buffer_kernel::size_int  my_id;
            uint64_t  my_id;
            BufferSync *buffer_sync_;
            mutex_buffer<Mutex> *mutex_buff_;

            //insert all task_id  for worker controls tasks.
            //pthread_t ** task_p_tid;
            std::vector<pthread_t> p_tid_task_vec;

            //id processes_id_register for thread
            struct slot_ocl *s_ocl;
            struct data_ocl_process<MAPPED_FILE> *d_ocl_processes;
            typename slot_ocl_thread<BufferSync, MAPPED_FILE>::
            load_ocl_system_type *load_ocl_system_;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };


}


#endif /* CONTROLLER_THREAD_HPP_ */
