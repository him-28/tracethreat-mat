#ifndef INTERNET_TRACETHREAT_INFECTED_TRACETHREAT_HPP
#define INTERNET_TRACETHREAT_INFECTED_TRACETHREAT_HPP

#include "internet/tracethreat/tracethreat.hpp"

namespace internet
{

    namespace tracethreat
    {

        class infected_tracethreat : public tracethreat
        {

            public:


                typedef scan_threat::InfectedFileInfoRequest MessageRequestType;
                typedef scan_threat::InfectedFileInfoResponse MessageResponseType;
								typedef infected_controller//<MessageRequestType, MessageResponseType>  
									infected_controller_type;

                infected_tracethreat(): 
								  infected_controller_(new infected_controller()){ }


                ~infected_tracethreat() { }

								bool initial_engine(std::string dest_ip, int port){
										return infected_controller_->initial_engine(dest_ip, port);
								}

                bool send(MessageRequestType*  msgReq, MessageResponseType * msgResp) {
                    return infected_controller_->send(msgReq, msgResp);
                }

								internet::tracethreat::
                tracethreat_controller *
                 get_tracethreat(){
										return infected_controller_;
								 }

								void loop(){
								   infected_controller_->loop();
								}
                
                void received(MessageResponseType&   msgs) {
                     //rpc_controller_->received(msgs);
                 }
                
            private:

                infected_controller_type * infected_controller_;

        };//rpc_tracethreat


        REGISTER_TRACETHREAT(infected_tracethreat);


    }//rpc


}//security


#endif /* INTERNET_SECURITY_RPC_TRACETHREAT_HPP */
