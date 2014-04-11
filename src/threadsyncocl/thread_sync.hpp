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
#include <boost/tuple/tuple.hpp>

// internal
#include "threadsyncocl/buffer_sync.hpp"
#include "threadsyncocl/semaphore_controller.hpp"
#include "threadsyncocl/thread_barrier_controller.hpp"

#include "threadsyncocl/thread_controller.hpp"

#include "memory/file_shm_handler.hpp"

//
#include "utils/logger/clutil_logger.hpp"
#include "utils/file_calculate.hpp"

#include "ocl/cl_bootstrap.hpp"

//PE SCANNING
//#include "filetypes/pe_file_controller.hpp"

namespace controller
{
    template<typename MAPPED_FILE>
    struct data_ocl_process;

    template<typename Buffer, typename MAPPED_FILE>
    class  BufferSync;

    template<typename BufferSync, typename MAPPED_FILE>
    class thread_sync;
}

/*
namespace filetypes
{

    template<typename MAPPED_FILE>
    class pe_file_controller;


}
*/

namespace controller
{

    using namespace utils;
		using namespace filetypes;

    namespace h_util = hnmav_util;

    namespace shm_memory = memory;

    namespace dstr   = data_structure;
    namespace kernel_ocl = hnmav_kernel;

    template<typename BufferSync>
    class ibuffer_sync
    {
        public:
            virtual boost::shared_ptr<BufferSync>& buffer_ocl() = 0;
            virtual ibuffer_sync& start_processes() = 0;
    };

    template<typename BufferSync, typename MAPPED_FILE>
    class thread_sync : public ibuffer_sync<BufferSync>
    {

        private:
            boost::shared_ptr<BufferSync> buffer_sync;

            //thread controller
            typedef BufferSync      buffer_sync_type;
						typedef mutex_buffer<Mutex>  mutex_buff;

            //typedef buffer_kernel::size_int size_type;
            typedef boost::shared_ptr<comm_thread_buffer<buffer_sync_type, MAPPED_FILE> > thread_ptr;
						typedef boost::shared_ptr<slot_ocl_thread<buffer_sync_type, MAPPED_FILE> > thread_ocl_ptr;

            //     std::string  *file_path;
            thread_ptr   *thread_array_ptr;

            buffer_sync_type *buff_sync_internal;

						mutex_buff   *mutex_sync_internal;
            //size_type thread_id;

            std::vector<thread_ptr> thread_ptr_vec;
            // pointer handler vector of shared_ptr of threads.
            std::vector<thread_ptr> *thread_pv_ptr;

						//Slot ocl thread
						std::vector<thread_ocl_ptr> thread_ocl_ptr_vec;
						std::vector<thread_ocl_ptr> *thread_ocl_pv_ptr;

						//task_id (thread_t)
						std::vector<pthread_t> p_tid_task_vec;

            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;


						
						 
        public:

						//ocl support
            typedef kernel_ocl::cl_load_system<kernel_ocl::clutil_platform,
                    dstr::dstr_def::work_groupitems,
                    std::vector<boost::unordered_map<char, size_t> >,
                    dstr::actire_parallel<char,
                    size_t,
                    boost::unordered_map<char, size_t>,
                    std::vector<boost::unordered_map<char, size_t> > >
                    >	 load_ocl_system_type;

            typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm *
                mapstr_shm_ptr;
            //mapstr_shm_type mapstr_shm_ptr;
						typename thread_sync<BufferSync, MAPPED_FILE>::load_ocl_system_type * 
								load_ocl_system;
	

            thread_sync();

            boost::shared_ptr<BufferSync>& buffer_ocl(){ 
							//TODO: Interface supported other class.
						};

            //boost::tuple<struct shm_memory::data_ocl_process<MAPPED_FILE>::size_int> get_thread_info();

			      boost::tuple<uint8_t> get_thread_info();

            ibuffer_sync<BufferSync>& start_processes();
											
            //insert file-shm mapped to create vector thread.
            std::vector<boost::shared_ptr<comm_thread_buffer<BufferSync,MAPPED_FILE> > >&
            init_syncocl_workload(typename shm_memory::
                    file_shm_handler<MAPPED_FILE>::map_str_shm& mapstr_shm,
                    std::map<const uint64_t , size_t> *map_file_size);
						//signature 
						bool add_sig_process(std::vector<char> * symbol_vec, std::vector<size_t> * state_vec);
						//OCL send to slot_ocl_thread
						bool add_load_ocl_system(typename thread_sync<BufferSync, MAPPED_FILE>::
							load_ocl_system_type * load_ocl_system,
							std::string * kernel_file_path_ptr);

    };

}

#endif /* THREAD_THREAD_SYNC_HPP */
