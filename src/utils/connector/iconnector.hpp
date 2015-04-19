#ifndef UTILS_CONNECTOR_ICONNECTOR_HPP
#define UTILS_CONNECTOR_ICONNECTOR_HPP

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
 * Initial for support object connector                   R.Chatsiri
 */
#include <vector>

namespace utils
{

    namespace connector
    {

        template<typename MessageType, typename DBConnector>
        class iconnector
        {

            public:

                virtual bool connector(const char *url, const char *port) = 0;

                virtual bool insert(MessageType& msg) = 0;

                virtual DBConnector *get_db()const = 0;

                virtual std::vector<MessageType *> get_msg_vec() = 0;
        };


    }

}


#endif /* UTILS_CONNECTOR_ICONNECTOR_HPP */
