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
 * - Integrated thread to multifile scanning system.              R.Chatsiri          25/03/2014
 */


#include "threadsyncocl/thread_controller.hpp"

namespace controller
{


    template<typename BufferSync>
    thread<BufferSync>::thread(bool detached)
        : runnable_(boost::shared_ptr<runnable>()), detached_(detached)
    {
        //logger
        //logger_ptr = &utils::clutil_logging<std::string, int>:: get_instance();
        //logger = logger_ptr->get();
        //logger->write_info("Thread, Thread init/ not start ", utils::format_type::type_header);
    }


    template<typename BufferSync>
    void *thread<BufferSync>::start_thread_runnable(void *p_void)
    {
        thread<BufferSync> *runnable_thread_ptr = static_cast<thread<BufferSync>* >(p_void);
        runnable_thread_ptr->result = runnable_thread_ptr->runnable_->run();
        //logger->write_info("Thread, start_thread_runnable"); // todo: Static cannot run with static_cast
        return runnable_thread_ptr->result;
    }

    template<typename BufferSync>
    void *thread<BufferSync>::start_thread(void *p_void)
    {
        thread<BufferSync> *thread_ptr = static_cast<thread<BufferSync>* >(p_void);
        thread_ptr->result = thread_ptr->run();
        //logger->write_info("Thread, start_thread"); // todo: Staitc cannot run with static_cast
        return thread_ptr->result;
    }

    template<typename BufferSync>
    bool thread<BufferSync>::thread_cancel()
    {
        if(is_current(get_thread_id())) {
            logger->write_info("Thread cancel, ID",
                    boost::lexical_cast<std::string>(get_thread_id()));
            pthread_cancel(get_thread_id());
            return true;
        }

        logger->write_info("Thread cannot cancel, ID",
                boost::lexical_cast<std::string>(get_thread_id()));

        return false;
    }

    template<typename BufferSync>
    void thread<BufferSync>::start()
    {
        logger->write_info("Thread, start() ");

        int status = pthread_attr_init(&thread_buffer_attr);

        if(status) {
            logger->write_info(" Attribute init fail ");
        }

        status = pthread_attr_setscope(&thread_buffer_attr, PTHREAD_SCOPE_SYSTEM);

        if(status) {
            logger->write_info(" setscope fail ");
        }

        if(!detached_) {
            if(!runnable_.get()) {
                status = pthread_create(&thread_buffer_id,
                        &thread_buffer_attr,
                        thread<BufferSync>::start_thread,
                        (void *)this);

                logger->write_info("Thread not detacted_, ID",
                        boost::lexical_cast<std::string>(get_thread_id())); // add argument

                if(status) {
                    logger->write_info(" Cannot create start_thread ");
                }
            } else {
                status = pthread_create(&thread_buffer_id,
                        &thread_buffer_attr,
                        thread<BufferSync>::start_thread_runnable,
                        (void *)this);
                logger->write_info("Thread, ID : "); // add argument

                if(status) {
                    logger->write_info(" Cannot create start_thread_runnable ");
                }

            }
        }

        status = pthread_attr_destroy(&thread_buffer_attr);

        if(status) {
            logger->write_info(" cannot destroy ");
        }
    }

    template<typename BufferSync>
    void *thread<BufferSync>::join()
    {
        int status = pthread_join(thread_buffer_id, NULL);

        if(status) {
            logger->write_info(" Cannot join thread_buffer_id ");
        }
    }

    template<typename BufferSync>
    void  thread<BufferSync>::set_completed()
    {
        // join completed
    }


    template<typename BufferSync>
    typename thread<BufferSync>::id_t  thread<BufferSync>::get_thread_id()
    {
        return pthread_self();
    }

    template<typename BufferSync>
    inline bool thread<BufferSync>::is_current(thread<BufferSync>::id_t t)
    {
        return pthread_equal(pthread_self(), t);

    }

    //-- Communication thread buffer
    template<typename BufferSync, typename MAPPED_FILE>
    void *comm_thread_buffer<BufferSync, MAPPED_FILE>::run()
    {
        //[x]thread write status in vector of struct name slot_ocl. Intial in thread_sync
        //[x]insert data from map_str_shm to vector of char. Intial in thread_sync
        //[/]call opencl machanism run opencl
        //[/]thread  check stust in vector result and slot_ocl have status completed.

        //Mutex buffer locks mutex,

        logger->write_info("Comm_thread_buffer::run(), Before to critical section.",
                utils::format_type::type_center);

        uint64_t found_size = 0;
        uint8_t  summary_status = 0;

        //loop thread check
        while(true) {

            mutex_buff_->lock_request();

            BufferSync *buff_sync = buffer_sync_;
            struct data_ocl_process<MAPPED_FILE> *docl_processes = buff_sync->buff;
            struct data_ocl_process<MAPPED_FILE>::map_md5_type  map_tid =
                            docl_processes->map_fmd5_slot_ocl;

            std::map<uint64_t, struct slot_ocl *>::iterator iter_tid;
            iter_tid =  map_tid.find(my_id);

            logger->write_info("comm_thread_buffer::run(), Pthread_self",
                    boost::lexical_cast<std::string>(pthread_self()));

            logger->write_info("comm_thread_buffer::run(), MD5_Specific Thread",
                    boost::lexical_cast<std::string>(my_id));


            if(iter_tid != map_tid.end()) {
                std::pair<uint64_t, struct slot_ocl *>  pair_tid = *iter_tid;
                struct slot_ocl *s_ocl = pair_tid.second;

                logger->write_info("Scan start point ",
                        boost::lexical_cast<std::string>(s_ocl->start_point));
                logger->write_info("Scan  end  point ",
                        boost::lexical_cast<std::string>(s_ocl->end_point));

                std::vector<uint8_t> *index_result = &docl_processes->index_binary_result;
                //check on ranking of star_point to end_point
                std::vector<uint8_t>::iterator iter_nresult = index_result->begin();

                std::advance(iter_nresult, s_ocl->start_point);

                for(;; ++iter_nresult) {
                    size_t index = iter_nresult - index_result->begin();
                    uint8_t value = *iter_nresult;


                    //logger->write_info("comm_thread_buffer::run(), status is zero");
                    //Check value not null and Index founds virus.
                    if(value == utils::infected_found) {

                        found_size++;

												logger->write_info_test("comm_thread_buffer::run(), value set",
																boost::lexical_cast<std::string>(value));

                        logger->write_info_test("comm_thread_buffer::run(), found size",
                                boost::lexical_cast<std::string>(found_size));

                        logger->write_info_test("comm_thread_buffer::run(), index_matching",
                                boost::lexical_cast<std::string>(index));

                        uint8_t sigtype = utils::pe_file; //delete in future
                        sig_shm_->verify_signature(sigtype,
                                value,
                                index,
                                &docl_processes->binary_hex,
                                &docl_processes->symbol_hex);

                    }

                    if(index == s_ocl->end_point) {
                        //TODO: Take loggic for check with another method.
                        logger->write_info_test("comm_thread_buffer::run(), found size",
                                boost::lexical_cast<std::string>(found_size));


                        //Make condition when found data.
                        s_ocl->status = utils::infected_fist_step;

                        //control state success or not.
                        ++summary_status;
                        //TODO: Thread wait, this->thread_cancel();

                        logger->write_info_test("comm_thread_buffer::run(), end_point check");
                        break;
                    }
                }

            }//if find map

            mutex_buff_->unlock_request();

            logger->write_info("-- comm_thread_buffer::run(), End of critical section --");

            if(summary_status == map_tid.size()) {

                logger->write_info("Comm_thread_buffer::run(), Task end, completed jobs",
                        utils::format_type::type_center);
                //this->thread_cancel();
                break;
            }

        }//while(true) loop

    }

    template<typename BufferSync, typename MAPPED_FILE>
    comm_thread_buffer<BufferSync, MAPPED_FILE>::~comm_thread_buffer()
    {
        delete buffer_sync_;
        delete mutex_buff_;
        delete s_ocl;
        delete d_ocl_processes;
    }


    template<typename BufferSync, typename MAPPED_FILE>
    void *slot_ocl_thread<BufferSync, MAPPED_FILE>::run()
    {
        logger->write_info("slot_ocl_thread::run(), start...");
        //[] start OCL and call function sends data to buffer of OCL.
        //[] check thread completed. check from size of thread. and map contain slot_ocl structure.
        uint64_t thread_size = 0;

        while(true) {
            mutex_buff_->lock_request();

            BufferSync *buff_sync = buffer_sync_;
            struct data_ocl_process<MAPPED_FILE> *docl_processes = buff_sync->buff;
            struct data_ocl_process<MAPPED_FILE>::map_md5_type  map_tid =
                            docl_processes->map_fmd5_slot_ocl;

            std::map<uint64_t, struct slot_ocl *>::iterator iter_tid;

            thread_size = map_tid.size();

            for(iter_tid = map_tid.begin(); iter_tid != map_tid.end(); ++iter_tid) {
                //logger->write_info_test("slot_ocl_thread::run()");
                std::pair<uint64_t, struct slot_ocl *>  pair_tid =  *iter_tid;

                struct slot_ocl *s_ocl = pair_tid.second;

                if(s_ocl->status == utils::infected_fist_step) {
                    //delete thread from thread_id ( file_name_md5)
                    --thread_size;
                    //logger->write_info_test("substract...");
                }

            }

            mutex_buff_->unlock_request();

            if(thread_size == 0) {
                logger->write_info("slot_ocl_thread::run(), Worker break, completed jobs.");
                //this->thread_cancel();
                break;
            }

        }
    }

    template<typename BufferSync, typename MAPPED_FILE>
    slot_ocl_thread<BufferSync, MAPPED_FILE>::~slot_ocl_thread()
    {
        delete buffer_sync_;
        delete mutex_buff_;
        delete s_ocl;
        delete d_ocl_processes;
        delete load_ocl_system_;
    }


    // Explicitly instance

    typedef BufferSync<struct data_ocl_process<MAPPED_FILE_PE>, MAPPED_FILE_PE> buff_sync;

    template class thread<buff_sync>;
    template class thread<int>;
    template class comm_thread_buffer<buff_sync, MAPPED_FILE_PE >;
    template class slot_ocl_thread<buff_sync, MAPPED_FILE_PE >;

}


