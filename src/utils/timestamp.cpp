#include "utils/timestamp.hpp"

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

namespace utils
{

    timestamp::timestamp()
    {
				
    }

    std::string timestamp::initial_timezone()
    {

				return "";
    }

    std::string timestamp::get_system_time()
    {

        now = boost::posix_time::second_clock::local_time();
        utc = boost::posix_time::second_clock::universal_time();

        boost::posix_time::time_duration tz_offset = (now - utc);

        output_facet = new boost::local_time::local_time_facet();

        ss.imbue(std::locale(std::locale::classic(), output_facet));

        output_facet->format("%H:%M:%S");

        ss.str("");

        ss << tz_offset;

        boost::local_time::time_zone_ptr    zone(new boost::local_time::posix_time_zone(ss.str().c_str()));

        boost::local_time::local_date_time  ldt = boost::local_time::local_microsec_clock::local_time(zone);

        output_facet = new boost::local_time::local_time_facet();

        ss.imbue(std::locale(std::locale::classic(), output_facet));

        output_facet->format("%Y-%m-%d %H:%M:%S%f%Q");
        ss.str("");
        ss << ldt;
        return ss.str();
    }

}

