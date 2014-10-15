/*
* Copyright 2014 Chatsiri Rattana.
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
 *  -UUID support SHM of file in memory mode.
 *  -UUID Internal and External file scanning.            R.Chatsiri       28/08/2014
 *
 */

#include <pthread.h>

#include "utils/uuid_generator.hpp"
#include "boost/lexical_cast.hpp"
#include <sstream>

namespace utils{

const std::string  uuid_generator::generate(uint64_t  tid)
{
		this->tid_ = tid;
	  if(search_thread_depend(tid))
		{
				return generate();	
		}	 
	return std::string("");
}

std::string uuid_generator::generate()
{
			 std::string uuid_str;
		   boost::uuids::uuid uid;
    	 std::stringstream ss;
			 ss << uid; 
			 return ss.str();
}

bool uuid_generator::search_thread_depend(uint64_t  tid)
{
	  std::map<pthread_t, std::string>::iterator iter_uuid_map;// = uuid_map.find(tid);
		for(iter_uuid_map = uuid_map.begin(); 
				iter_uuid_map != uuid_map.end();
				++iter_uuid_map)
		{
					std::pair<pthread_t, std::string> pair_id = *iter_uuid_map;
		      if(pthread_equal(tid, pair_id.first))
					{
							return true;
					}
		}
		return false;
}

}


