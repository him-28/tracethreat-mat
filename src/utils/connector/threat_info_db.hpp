#ifndef UTILS_CONNECTOR_THREAT_INFO_DB_HPP
#define UTILS_CONNECTOR_THREAT_INFO_DB_HPP

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
 * Threat infomation after scan file from scan_client will send to DRocksDB
 *                                                        R.Chatsiri       16/11/2014
 */

#include "utils/logger/clutil_logger.hpp"

#include "utils/connector/mongo_connector.hpp"

#include "msg/message_tracethreat.pb.h"

namespace utils
{

    namespace connector
    {
        typedef boost::shared_ptr<scan_threat::InfectedFileInfoRequest> msg_threat_info_ptr;

        template<typename MessageThreatInfoPointer =
        boost::shared_ptr<scan_threat::InfectedFileInfoRequest> >
        class threat_info_db : public mongo_connector<MessageThreatInfoPointer,mongo::DBClientConnection>
        {
            public:

                bool prepare_connection();

                virtual bool insert(MessageThreatInfoPointer& msg);

            private:
                const char *db_name;
                const char *url;
                const char *port;

                boost::shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
                utils::clutil_logging<std::string, int>    *logger;

        };

    }


}


#endif /* UTILS_CONNECTOR_THREAT_INFO_DB_HPP */
