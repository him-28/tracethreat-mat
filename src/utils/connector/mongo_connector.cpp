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
 *  -Connector connects to mongodb                        R.Chatsiri
 */

#include <string>
#include <iostream>

#include "utils/connector/mongo_connector.hpp"

namespace utils
{

    namespace connector
    {

        template<typename MessageType, typename DBConnector>
        bool mongo_connector<MessageType, DBConnector>::connection(const char *url, const char *port)
        {
            std::string err_msg;

            if(conn.connect(std::string(url)+std::string(":")+port, err_msg)) {
                logger->write_info("Connection error ", err_msg);
								flag_connect = false;
                return flag_connect;
            }
						logger->write_info("Connection completed.");
						flag_connect = true;
            return flag_connect;
        }

				/*
				template<typename MessageType, typename DBConnector>
				bool mongo_connector<MessageType, DBConnector>::insert(MessageType & msg){
						MessageType  msg_bson = msg;
						conn.insert(db_name, BSON()

				}
				*/

				template<typename MessageType, typename DBConnector>
				bool mongo_connector<MessageType, DBConnector>::database(const char * db_name){
						this.db_name =  db_name;
				}

    }


}
