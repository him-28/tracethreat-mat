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
 *- Detail of infected file controller.
 *                                                        R.Chatsiri       04/03/2015
 */


#include "internet/tracethreat/infected_controller.hpp"


namespace internet
{


    namespace tracethreat
    {

        //template<typename MessageRequestType, typename MessageResponseType>
        bool infected_controller:://<MessageRequestType, MessageResponseType>::
        initial_engine(std::string dest_ip, int port)
        {
						
            loop_    = new rpc::EventLoop();
            channel_ = new rpc::RpcChannel(loop_, dest_ip, port);
            remoteService_ = new scan_threat::InfectedFileInfoService::Stub(channel_);
            LOG(INFO)<<"RPC-Service : Infected Controlller initial...";
						
        }//initial_engine
				
				void received(scan_threat::InfectedFileInfoResponse * msgs){
						LOG(INFO)<<"Test received id(UUID) : "<<msgs->uuid();
				}
				

        //template<typename MessageRequestType, typename MessageResponseType>
        bool infected_controller:://<MessageRequestType, MessageResponseType>::
        send(MessageRequestType * msgReq, MessageResponseType * msgResp)
        {
						//MessageRequestType   _msgReq;
						//MessageResponseType * _msgResp;
						LOG(INFO)<<"RPC-Service : Request UUID : " << msgReq->uuid();

            LOG(INFO)<<"RPC-Service : Send message to RPC-Server";

              remoteService_->InfectedFileInfo(NULL, msgReq,  msgResp, NewCallback(
              &received,
              msgResp));
        }//send


				void infected_controller::loop(){
                   loop_->loop();
				}
 
				void infected_controller::break_loop(){
                   loop_->break_loop();
				}
                                    

        /*template<typename MessageRequestType, typename MessageResponseType>
        void infected_controller:://<MessageRequestType, MessageResponseType>::
        received(MessageResponseType * msgs)
        {
            LOG(INFO)<<"RPC-Service : Received message from RPC-Server";

        }
        /* */

        //template<typename MessageRequestType, typename MessageResponseType>
        infected_controller:://<MessageRequestType, MessageResponseType>::
        ~infected_controller()
        {
            LOG(INFO)<<"Deleted  RPC-Service";
            //loop_->~EventLoop();
        }
				/*
			  template class infected_controller<scan_threat::InfectedFileInfoRequest, 
                                           scan_threat::InfectedFileInfoResponse>;
        */
    }//rpc


}//internet
