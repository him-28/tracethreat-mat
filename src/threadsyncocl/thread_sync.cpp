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
 * - Change to SyncOCL                                               R.Chatsiri   01/04/2014
 * - Initial multithread run on comm_thread_buffer                   R.Chatsiri   04/04/2014
 */

#include <string>
#include <iostream>

#include "threadsyncocl/thread_sync.hpp"


namespace controller
{


    template<typename BufferSync, typename MAPPED_FILE>
    thread_sync<BufferSync, MAPPED_FILE>::thread_sync()
    {

    }

    template<typename BufferSync, typename MAPPED_FILE>
    //std::vector<boost::shared_ptr<comm_thread_buffer<BufferSync, MAPPED_FILE> > >&
    bool thread_sync<BufferSync, MAPPED_FILE>::init_syncocl_workload(typename shm_memory::
            file_shm_handler<MAPPED_FILE>::map_str_shm& mapstr_shm,
            std::map<const uint64_t , size_t> *map_file_size,
            signature_shm_type *sig_shm)
    {
        logger->write_info("thread_sync::init_syncocl_workload(), Start initial workload");
        uint64_t file_size_summary = 0;

        //buffer data for multithread.
        buff_sync_internal  = new BufferSync();
        //mutex for multithread.
        mutex_sync_internal = new mutex_buffer<Mutex>();
        //initial  mutex before send to comm_thread_buffer
        mutex_sync_internal->init();

        if(mapstr_shm.size() == 0)
            return false;

        typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm::iterator iter_mapstr_shm;
        typename shm_memory::file_shm_handler<MAPPED_FILE>::binary_string_shm    *binarystr_shm;

        for(std::map<const uint64_t , size_t>::iterator iter_map = map_file_size->begin();
                iter_map != map_file_size->end();
                ++iter_map) {
            std::pair<const uint64_t, size_t> pair_file_size = *iter_map;
            size_t size_hex = pair_file_size.second;
            file_size_summary += size_hex;
        }

        logger->write_info("thread_sync::init_syncocl_workload(), File Size Summary ",
                boost::lexical_cast<std::string>(file_size_summary));


        for(iter_mapstr_shm  = mapstr_shm.begin();
                iter_mapstr_shm != mapstr_shm.end();
                ++iter_mapstr_shm) {

            typename shm_memory::file_shm_handler<MAPPED_FILE>::
            value_types_str pair_int_str = *iter_mapstr_shm;

            // file_name_md5 : It's uid instance processes id
            // Global struct data(file-shm, vector hex char and vector result) initials
            // for processing buff_sync_internal.
            const uint64_t file_name_md5 = pair_int_str.first;

            //initial data for file md5 name per struct name slot_ocl
            buff_sync_internal->filemd5_regis_ocl(file_name_md5);

            std::pair<const uint64_t, size_t>  pair_file_size = *map_file_size->find(file_name_md5);

            size_t size_hex = pair_file_size.second;

            binarystr_shm = &pair_int_str.second;

            //const char *  data_char_hex = binarystr_shm->c_str(); : get const string from file-shm
            //insert binary hex data to vector
            //shm size : binarystr_shm->size()

            if(!buff_sync_internal->write_binary_hex_ocl(binarystr_shm->c_str(),
                    size_hex,
                    file_name_md5)) {
                logger->write_info("thread_sync::init_syncocl_workload(), Critical Problems");
                logger->write_info("thread_sync::init_syncocl_workload(), Write binary file problem");
                break;
            }

            logger->write_info("thread_sync::init_syncocl_workload(), write binary file completed.");

            //TODO: Implement thread_id (Not file_name_md5 )
            thread_ptr_vec.push_back(
                    boost::shared_ptr<comm_thread_buffer<BufferSync, MAPPED_FILE> >(
                            new comm_thread_buffer<BufferSync,MAPPED_FILE>
                            (
                                    file_name_md5
                                    ,buff_sync_internal
                                    ,mutex_sync_internal
                                    ,sig_shm
                            )
                    ) //[thread_id]
            );

            logger->write_info("thread_sync::init_syncocl_workload(), Push thread task completed.",
                    boost::lexical_cast<std::string>(file_name_md5));


        }//end for loop

        // controll multithread scanning.
        thread_ocl_ptr_vec.push_back(
                boost::shared_ptr<slot_ocl_thread<BufferSync, MAPPED_FILE> >(
                        new slot_ocl_thread<BufferSync, MAPPED_FILE>
                        (
                                this->load_ocl_system,
                                buff_sync_internal,
                                mutex_sync_internal
                        )
                )
        );

        logger->write_info("thread_sync::init_syncocl_workload(), Push worker controller completed.");

        thread_pv_ptr = &thread_ptr_vec;
        //Internal process reference to ::start_processes() member function.
        //thread_ocl_pv_ptr reference object on start_processes() member function.
        thread_ocl_pv_ptr = &thread_ocl_ptr_vec;

        return true;
    }


    template<typename BufferSync, typename MAPPED_FILE>
    ibuffer_sync<BufferSync>& thread_sync<BufferSync, MAPPED_FILE>::start_processes()
    {

        logger->write_info("Start, thread_sync::::start_processes() ", utils::format_type::type_center);

        //define Thread run / mutex at here
        typename std::vector<thread_ptr>::iterator iter_threads;

        //define iter run
        typename std::vector<thread_ocl_ptr>::iterator iter_ocl_thread;


        //intial worker
        for(iter_ocl_thread = thread_ocl_pv_ptr->begin();
                iter_ocl_thread != thread_ocl_pv_ptr->end();
                ++iter_ocl_thread) {
            boost::shared_ptr<s_ocl_thread_worker> s_ocl = *iter_ocl_thread;
            //s_ocl->set_tid_task(p_tid_task_vec);
            s_ocl->start();
        }

        logger->write_info("thread_sync::::start_processes(), Start worker success.");

        //start threads
        for(iter_threads = thread_pv_ptr->begin();
                iter_threads != thread_pv_ptr->end();
                ++iter_threads) {
            // get thread prompt.
            boost::shared_ptr<comm_thread_buff> ct_buff = *iter_threads;
            ct_buff->start();
            //get thread id send is task_id for worker name : slot_ocl_thread.
            //p_tid_task_vec.push_back(ct_buff->get_thread_id());
        }

        logger->write_info("thread_sync::::start_processes(), Start threader success.");

        //join threads/ Join
        for(iter_threads = thread_pv_ptr->begin();
                iter_threads != thread_pv_ptr->end();
                ++iter_threads) {
            // get thread prompt.
            boost::shared_ptr<comm_thread_buff> ct_buff = *iter_threads;
            ct_buff->join();
            break;
        }

        for(iter_ocl_thread = thread_ocl_pv_ptr->begin();
                iter_ocl_thread != thread_ocl_pv_ptr->end();
                ++iter_ocl_thread) {
            boost::shared_ptr<s_ocl_thread_worker> s_ocl = *iter_ocl_thread;
            //s_ocl->set_tid_task(p_tid_task_vec);
            s_ocl->join();
        }


        logger->write_info("End, thread_sync::::start_processes() ", utils::format_type::type_center);
    }

    template<typename BufferSync, typename MAPPED_FILE>
    boost::tuple<uint8_t>
    thread_sync<BufferSync,MAPPED_FILE>::get_thread_info()
    {
        return 	boost::tuple<uint8_t>(thread_ptr_vec.size());
    }

    template<typename BufferSync, typename MAPPED_FILE>
    bool  thread_sync<BufferSync, MAPPED_FILE>::add_load_ocl_system(
            typename thread_sync<BufferSync, MAPPED_FILE>::load_ocl_system_type
            *load_ocl_system,
            std::string *kernel_file_path_ptr,
            std::vector<char> *symbol_vec,
            std::vector<size_t>   *state_vec)
    {

        logger->write_info("thread_sync::add_load_ocl_system(), Path-OCL-file ", *kernel_file_path_ptr);

        this->load_ocl_system = load_ocl_system;
        this->load_ocl_system->set_opencl_file(*kernel_file_path_ptr);
        this->load_ocl_system->cl_load_platform();
        this->load_ocl_system->cl_load_memory();

        //next steps load symbol and state with add_sig_process member function.
        //convert state size_t to int for ocl
        //Plan-00004 : Take fast speed convert from size_t to int type.
        std::vector<int>  state_vec_convert;
        //state_vec_convert.resize(state_vec->size());
        std::vector<size_t>::iterator iter_state_vec;

        for(iter_state_vec = state_vec->begin();
                iter_state_vec != state_vec->end();
                ++iter_state_vec) {
            std::string state_str = boost::lexical_cast<std::string>(*iter_state_vec);

            logger->write_info("thread_sync::add_load_ocl_system(), state convert ",
                    state_str);

            state_vec_convert.push_back(boost::lexical_cast<int>(state_str));
        }

        buff = buff_sync_internal->buff;
        //set binary_hex send to OCL
        std::vector<char>  binary_hex = buff->binary_hex;

        // set binary_hex end-result to OCL. OCL set success index of  vector contained.
        std::fill(buff->index_binary_result.begin(),
                buff->index_binary_result.end(),
                utils::infected_not_found);

        std::vector<uint8_t>   *binary_result = &buff->index_binary_result;

        //set symbol to buffer_internal. For check matching with OCL.
        //TODO: Move to setter member function.
        buff->symbol_hex = *symbol_vec;

        //Send all data to OCL for calculating and matching function.
        this->load_ocl_system->cl_process_buffer(*symbol_vec,
                state_vec_convert,
                binary_hex,
                *binary_result);


        this->load_ocl_system->cl_build_memory();
        this->load_ocl_system->cl_load_commandqueue();
        this->load_ocl_system->cl_process_commandqueue(binary_result);

        return true;
    }

    //Cannot Load process Why?
    template<typename BufferSync, typename MAPPED_FILE>
    bool thread_sync<BufferSync, MAPPED_FILE>::add_sig_process(
            std::vector<char> *symbol_vec,
            std::vector<size_t>   *state_vec)
    {
        if(this->load_ocl_system == NULL) {
            logger->write_info("thread_sync::add_sig_process(), Load OCL System is NULL");
            return false;
        }

        return true;
    }

    template<typename BufferSync, typename MAPPED_FILE>
    thread_sync<BufferSync, MAPPED_FILE>::~thread_sync()
    {
        delete thread_array_ptr;
        delete mutex_sync_internal;
        delete thread_pv_ptr;
    }


    template class controller::thread_sync<
    BufferSync<struct data_ocl_process<MAPPED_FILE_PE>, MAPPED_FILE_PE >,
               MAPPED_FILE_PE
               >;

}
