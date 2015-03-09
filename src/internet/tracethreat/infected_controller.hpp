#ifndef INTERNET_TRACETHREAT_INFECTED_CONTROLLER_HPP
#define INTERNET_TRACETHREAT_INFECTED_CONTROLLER_HPP

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
 *- Send infected file detail from RPC-Client service to RPC-Server.
 *                                                        R.Chatsiri       04/03/2015
 */

#include <boost/shared_ptr.hpp>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

//central message use internet and tbbscan using.
#include "msg/message_tracethreat.pb.h"

#include <rpc/RpcChannel.hpp>
#include <rpc/EventLoop.hpp>
#include <rpc/Encodes.hpp>


namespace internet
{

    namespace tracethreat
    {

				//class rpc::EventLoop;

			  //template<typename MessageRequestType, typename MessageResponseType>
        class infected_controller;

        //template<typename MessageRequestType, typename MessageResponseType>
        class tracethreat_controller;


        //template<typename MessageRequestType, typename MessageResponseType>
        class tracethreat_controller
        {
            public:

                typedef scan_threat::InfectedFileInfoRequest MessageRequestType;
                typedef scan_threat::InfectedFileInfoResponse MessageResponseType;

                virtual bool	initial_engine(std::string dest_ip, int port) = 0;

                virtual bool send(MessageRequestType *  msgReq, MessageResponseType * msgResp) = 0;

                virtual void loop() = 0;

                //virtual void received(MessageResponseType * msgs) = 0;
								/*
                virtual internet::tracethreat::
                tracethreat_controller<MessageRequestType, MessageResponseType> *
                get_tracethreat() = 0;
								*/
                ~tracethreat_controller() { }

        };

        //template<typename MessageRequestType, typename MessageResponseType>
        class infected_controller : public tracethreat_controller//<MessageRequestType, MessageResponseType>
        {
            public:

                typedef scan_threat::InfectedFileInfoRequest MessageRequestType;
                typedef scan_threat::InfectedFileInfoResponse MessageResponseType;

                infected_controller() { }

                bool	initial_engine(std::string dest_ip, int port);

                bool send(MessageRequestType * msgReq, MessageResponseType * msgResp);

                void loop();

                //void received(MessageResponseType * msgs);

							  ~infected_controller();//{ }	
               	/*
                internet::tracethreat::
                tracethreat_controller<MessageRequestType, MessageResponseType> *
                get_tracethreat(){ return this; }
								*/
            private:

                rpc::EventLoop *loop_;
                rpc::RpcChannel *channel_;
                scan_threat::InfectedFileInfoService::Stub   *remoteService_;
        };//infected_controller


    }//rpc

}//internet



#endif /* INTERNET_RPC_INFECTED_CONTROLLER_HPP */
