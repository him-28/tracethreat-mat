#ifndef MEMORY_FILE_MEM_HANDLER__HPP
#define MEMORY_FILE_MEM_HANDLER__HPP

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

/*  Titles			                                          Authors	         Date
 * - File memory controllers interprocesses between scanning and memory buffer
 *                                                        R.Chatsiri       07/02/2014
 * - Register class for initial memory for file buffer    R.Chatsiri       07/02/2014
 */                       

//external 
#include <boost/interprocesses/detail/config_begin.hpp>
#include <boost/interprocesses/file_mapping.hpp>
#include <boost/interprocesses/mapped_region.hpp>

//internal
#include "memory/file_shm_base.hpp"

namespace memory{

	template<typename MAPPED_FILE>
	class file_shm_handler : public file_shm_base<MAPPED_FILE>{

		public:
		typename boost::shared_ptr<MAPPED_FILE> mapped_file;
		typename boost::shared_ptr<struct file_desc_shm> fdesc_shm_str;		

		virtual fdesc_shm_str file_desc();

		bool  create_mapped();

		bool  set_mapped_file(const char * file_name);
				
		private:
		mapped_file  mapped_file_sptr;
				
		boost::interprocesses::file_mapping  * fmapping;
		boost::interprocesses::mapped_region * mapping_reg;

		const char  * file_name;
		const size_t  file_size;
		
		protected:
		//TODO: Change type of vector to allocated with internal allocated did not standard.
		virtual std::vector<char> write_to_memory();

	  fdesc_shm_str fdes_shm_str_sptr; 	
		
		
		
	};

}

#endif
