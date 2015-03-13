#ifndef UTILS_CONNECTOR_MONGO_CONNECTOR_HPP
#define UTILS_CONNECTOR_MONGO_CONNECTOR_HPP

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
#include "utils/logger/clutil_logger.hpp"

#include "mongo/client/dbclient.h"

#include "utils/connector/iconnector.hpp"

namespace utils
{

    namespace connector
    {
        //using namespace mongo;
				//namespace utils = hnmav_util;

        template<typename MessageType, typename DBConnector = mongo::DBClientConnection>
        class mongo_connector : public iconnector<MessageType, DBConnector>
        {

            public :

                /**
                * @brief Message need to insert to mongodb. Must iterator of message.
                * Message_Insert[Message-A,Message-B,...,Message-N+1]
                *
                * @param msg  Message is Protocol Buffer.
                *
                * @return False, If data cannot insert to database.
                */
                virtual bool insert(MessageType& msg) = 0;

                /**
                * @brief Flag connector is false. It should connect database before operate database.
                */
                mongo_connector() : flag_connect(false) { }

            protected :


                /**
                * @brief Database connects to URL and port binding.
                *
                * @param url URL database such  127.0.0.1
                * @param port Port of MongoDB open to binding.
                *
                * @return False, If cannot initial database.
                */
                virtual bool connection(const char *url, const char *port);


                /**
                * @brief Database name is part of Mongo
                * [ DatabaseName [ collection[...[Document...]..] ]
                *
                * @param name  Name of database.
                *
                * @return If value not null.
                */
                virtual bool database(const char *name);

                /**
                * @brief Get Database connector use to another object.
                *
                * @return Object of connecting database. Purpose get connection object
                * after initial with IP,Port.
                */
                virtual DBConnector *get_db()const;

                /**
                * @brief Vector contain pointer of data insert to database in once time to insert.
                *
                * @return Vector contain pointer of message.
                */
                virtual std::vector<MessageType *> get_msg_vec();



            private:

                mongo::client::GlobalInstance instance;

                mongo::DBClientConnection  conn;

                const char *db_name;

                bool flag_connect;
					
								boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
							  utils::clutil_logging<std::string, int>    *logger;
        };


    }


}


#endif /* UTILS_MONGO_CONNECTOR_HPP */
