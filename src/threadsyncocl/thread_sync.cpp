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
 */

#include "threadsyncocl/thread_sync.hpp"


namespace controller
{


    template<typename BufferSync, typename MAPPED_FILE>
    thread_sync<BufferSync, MAPPED_FILE>::thread_sync()
    {
        // logger
        logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();
        //init value
        //thread_id = 0;
    }

		/*
    template<typename BufferSync, typename MAPPED_FILE>
    boost::shared_ptr<BufferSync, MAPPED_FILE>& thread_sync<BufferSync, MAPPED_FILE>::buffer_ocl()
    {
        return buffer_sync;
    }
		*/

    template<typename BufferSync, typename MAPPED_FILE>
    std::vector<boost::shared_ptr<comm_thread_buffer<BufferSync, MAPPED_FILE> > >&
    thread_sync<BufferSync, MAPPED_FILE>::init_syncocl_workload(typename shm_memory::
            file_shm_handler<MAPPED_FILE>::map_str_shm& mapstr_shm,
            std::map<const uint64_t , size_t> *map_file_size)
    {

        buff_sync_internal = new BufferSync();

        if(mapstr_shm.size() == 0)
            return thread_ptr_vec;

        typename shm_memory::file_shm_handler<MAPPED_FILE>::map_str_shm::iterator iter_mapstr_shm;	
				typename shm_memory::file_shm_handler<MAPPED_FILE>::binary_string_shm  *  binarystr_shm;


        for(iter_mapstr_shm  = mapstr_shm.begin();
                iter_mapstr_shm != mapstr_shm.end();
                ++iter_mapstr_shm) {

            typename shm_memory::file_shm_handler<MAPPED_FILE>::
							value_types_str pair_int_str = *iter_mapstr_shm;
            // file_name_md5 : It's uid instance processes id
            // Global struct data(file-shm, vector hex char and vector result) initials
            // for processing buff_sync_internal.
            const uint64_t file_name_md5 = pair_int_str.first;
					
            //initial data for thread per struct name slot_ocl
            buff_sync_internal->threadbuff_regis(file_name_md5);

            std::pair<const uint64_t, size_t>  pair_file_size = *map_file_size->find(file_name_md5);
						
            size_t size_hex = pair_file_size.second;

						std::cout<<"MD5 : " << file_name_md5 <<", size char hex : " << size_hex <<std::endl;

				    binarystr_shm = &pair_int_str.second;
						//const char *  data_char_hex = binarystr_shm->c_str(); : get const string from file-shm
            //insert binary hex data to vector
           	std::cout<<"thread_sync<BufferSync, MAPPED_FILE>::init_syncocl_workload,size : "<<
							binarystr_shm->size() <<std::endl;  
            if(buff_sync_internal->write_binary_hex(binarystr_shm->c_str(), size_hex, file_name_md5)) {
                //TODO: problem before return
            }

            thread_ptr_vec.push_back(
                    boost::shared_ptr<comm_thread_buffer<BufferSync, MAPPED_FILE> >(
                            new comm_thread_buffer<BufferSync,MAPPED_FILE>
                            (file_name_md5, buff_sync_internal)) //[thread_id]
            );
					  
						

        }

        thread_pv_ptr = &thread_ptr_vec;
        return thread_ptr_vec;
    }


    template<typename BufferSync, typename MAPPED_FILE>
    ibuffer_sync<BufferSync>& thread_sync<BufferSync, MAPPED_FILE>::start_processes()
    {

        logger->write_info("Thread processes ", h_util::format_type::type_center);
        typedef BufferSync  buffer_sync;
        typedef comm_thread_buffer<buffer_sync,MAPPED_FILE>  comm_thread_buff;


        /*
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
        */

        logger->write_info("End of Thread processes ", h_util::format_type::type_center);
    }

    template<typename BufferSync, typename MAPPED_FILE>
    boost::tuple<uint8_t> 
			thread_sync<BufferSync,MAPPED_FILE>::get_thread_info()
    {
        return 	boost::tuple<uint8_t>(thread_ptr_vec.size());
    }

    template class controller::thread_sync<
					BufferSync<struct data_ocl_process<MAPPED_FILE_PE>, MAPPED_FILE_PE >,
					MAPPED_FILE_PE 
		>;

}
