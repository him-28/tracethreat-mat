#ifndef UTILS_UUID_GENERATOR_HPP
#define UTILS_UUID_GENERATOR_HPP

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
 * - UUID generates number support thread_id.
 *   Utility for object support multithread            R.Chatsiri
 */

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>  
#include <map>

class uuid_generator{

public:

			void uuid_generate(){ };

	    const std::string generate(uint64_t  tid);

			std::string generate();

		  bool search_thread_depend(uint64_t tid);	
private:
	  std::map<uint64_t, std::string>  uuid_thread_map;
		uint64_t tid_;
};

#endif /* UTILS_UUID_GEN_HPP */
