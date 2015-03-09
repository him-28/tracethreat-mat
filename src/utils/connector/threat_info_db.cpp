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
 * Threat Infomation database supports scanning threat from TBBScanning.
 *                                                        R.Chatsiri       16/11/2014
 */

#include "mongo/client/dbclient.h"

#include "utils/connector/threat_info_db.hpp"

namespace utils
{

    namespace connector
    {
        template<typename MessageType>
        bool threat_info_db<MessageType>::prepare_connection()
        {
            return this->flag_connect;
        }

        template<typename MessageType>
        bool threat_info_db<MessageType>::insert(MessageType& msg_ptr)
        {

            const scan_threat::InfectedFileInfoRequest  threat_info =
                    msg_ptr;

            mongo::BSONObj  threat_info_bson =
                    mongo::BSONObjBuilder().append("encode_sign_type", threat_info.encode_sig_type())
                    .append("file_type", 			threat_info.file_type())
                    .append("status_result", 	threat_info.status_result())
                    .append("file_name", 			threat_info.file_name())
                    .append("binary",    			threat_info.binary()).obj();

            this->conn.insert(this->db_name, threat_info_bson);
            return true;
        }

    }


}
