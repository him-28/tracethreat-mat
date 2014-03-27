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
 * - Memberfunction sync changed design for supporting OCL-Sync      R.Chatsiri   25/03/2014
 */

#include "threadsyncocl/thread_sync.hpp"


namespace controller
{


    template<typename BufferSync>
    thread_sync<BufferSync>::thread_sync()
    {
        // logger
        logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();
        //init value
        thread_id = 0;
    }

    template<typename BufferSync>
    std::string thread_sync<BufferSync>::path()
    {
        return *file_path;
    }

    template<typename BufferSync>
    void thread_sync<BufferSync>::set_path(const char *path)
    {
        file_path = new std::string(path, strlen(path));
    }


    template<typename BufferSync>
    boost::shared_ptr<BufferSync>& thread_sync<BufferSync>::buffer_ocl()
    {
        return buffer_sync;
    }


    /**
    * @brief Thread_ptr_vec push shared_ptr of  comm_thread_buff in order to contain file of thread
    */
    template<typename BufferSync>
    std::vector<boost::shared_ptr<comm_thread_buffer<BufferSync> > >&
    thread_sync<BufferSync>::sync_init()
    {
        file_cal = new file_calculate<Extension>();
        std::string path_all = path();

        if(!file_cal->set_filepath(path_all.c_str())) {
            logger->write_info("File cannot get from path");
            return  thread_ptr_vec;
        }

        if(!file_cal->processes()) {
            logger->write_info("File cannot get/sorts in processes()");
            return thread_ptr_vec;
        }

        int thread_size = 0;
        // thread per file.
        file_cal->file_size();
        list_files = &file_cal->get_files2buffer();
        thread_size = list_files->size();

				buff_sync_internal = new BufferSync();
        // Thread will start on thread_new
        for(typename std::list<std::string>::iterator
                iter_file =  list_files->begin();
                iter_file != list_files->end();
                ++iter_file, thread_id++) {

            thread_ptr_vec.push_back(
                    boost::shared_ptr<comm_thread_buffer<BufferSync> >(
                            new comm_thread_buffer<BufferSync>
                            (thread_id, buff_sync_internal)) //[thread_id]
            );
        }

        thread_pv_ptr = &thread_ptr_vec;
        return thread_ptr_vec;
    }

    template<typename BufferSync>
    ibuffer_sync<BufferSync>& thread_sync<BufferSync>::sync_processes()
    {

        logger->write_info("Thread processes ", h_util::format_type::type_center);
        typedef BufferSync  buffer_sync;
        typedef comm_thread_buffer<buffer_sync>  comm_thread_buff;
        //define Thread run / mutex at here
        typename std::vector<thread_ptr>::iterator iter_threads;

        for(iter_threads = thread_pv_ptr->begin();
                iter_threads != thread_pv_ptr->end();
                ++iter_threads) {
            // get thread prompt.
            boost::shared_ptr<comm_thread_buff> ct_buff = *iter_threads;
            ct_buff->start();
            ct_buff->run();
        }

        logger->write_info("End of Thread processes ", h_util::format_type::type_center);
    }

    template<typename BufferSync>
    boost::tuple<buffer_kernel::size_int> thread_sync<BufferSync>::get_thread_info()
    {
        return 	boost::tuple<buffer_kernel::size_int>(thread_ptr_vec.size());
    }

    template class controller::thread_sync<BufferSync<buffer_kernel> >;

}
