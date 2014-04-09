#ifndef CONTROLLER_THREAD_REGIS_CONTROLLER_HPP
#define CONTROLLER_THREAD_REGIS_CONTROLLER_HPP

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

/*  Titles																													 Authors						Date
 * -  Thread register controller.                                 R.Chatsiri 
 *
 */

#include <boost/uuid.hpp>

class thread_regist_controller : public thread_controller
{

    public:
        thread_regist_controller();

        regist_uuid(const uint64_t uuid, pthread_t  thread_id);

        uint64_t get_uuid()const;

    private:

        bool create_uuid();

        std::map<uint64_t pthread_t> uuid_map;
};



#endif
