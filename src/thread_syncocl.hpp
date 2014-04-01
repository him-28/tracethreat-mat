#ifndef THREAD_SYNOCL_HPP
#define THREAD_SYNOCL_HPP

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
 * - Change to SyncOCL                                               R.Chatsiri   01/04/2014
 */

class controller
{
	 
	 template<typename BufferSyncOCL, typename MAPPED_FILE>
	 class thread_syncocl{
		public:
				typename file_shm_handler<MAPPED_FILE>::map_str_shm mapstr_shm_type;
				bool init_syncocl_workload(mapstr_shm_type & mapstr_shm);
				bool 
		private:
			  mapstr_shm_type * mapstr_shm_ptr;	

	 };

}


#endif /* THREAD_SYNOCL_HPP */
