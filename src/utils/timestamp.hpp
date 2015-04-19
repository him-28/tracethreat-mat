#ifndef UTILS_TIMESTAMP_HPP
#define UTILS_TIMESTAMP_HPP

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

/*  Titles																								Authors					 Date
 *
 */


#include <boost/date_time/local_time/local_time.hpp>

#include <sstream>
#include <string>

namespace utils{

   class timestamp{
			public:

					timestamp();
		
					std::string initial_timezone();

					std::string get_system_time();

			private:
					boost::posix_time::ptime  now;
          boost::posix_time::ptime  utc;
          boost::local_time::local_time_facet* output_facet;
          std::stringstream   ss;
	 };

}


#endif /* UTILS_TIMESTAMP_HPP */
