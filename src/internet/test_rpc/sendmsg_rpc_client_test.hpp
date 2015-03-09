#include "internet/tracethreat/load_tracethreat.hpp"
#include "internet/tracethreat/infected_controller.hpp"
#include "../../msg/message_tracethreat.pb.h"

class TraceThreatClientEngine : public ::testing::Test{

	protected :

  typedef scan_threat::InfectedFileInfoRequest MessageRequestType;
  typedef scan_threat::InfectedFileInfoResponse MessageResponseType;

	typedef internet::tracethreat::infected_controller//<MessageRequestType, MessageResponseType> 
		infected_controller_type;
 
  typedef internet::tracethreat::tracethreat_controller//<MessageRequestType, MessageResponseType>
    tracethreat_controller_type;

	virtual void SetUp(){
		
				LOG(INFO)<<"Server : Load RPC-Message engine";

				internet::tracethreat::get_tracethreat().reset( internet::tracethreat::create_tracethreat());
				
				if(internet::tracethreat::get_tracethreat().get() == NULL){
              LOG(INFO)<<"Server: System cannot intiail RPC-Message engine.";
				}//if


			  tracethreat_controller_ = internet::tracethreat::get_tracethreat()->get_tracethreat();

				if(tracethreat_controller_ == NULL){
            LOG(INFO)<<"Server : RPC-Message controller cannot intial";
				}

	}//setup


			  tracethreat_controller_type *  tracethreat_controller_;

};


TEST_F(TraceThreatClientEngine, InitialSendMessage){

  //Set message request to server.
  MessageRequestType msgReq;
  MessageResponseType * msgResp = new MessageResponseType;

	msgReq.set_uuid("0:0:0:11");

	tracethreat_controller_->initial_engine("127.0.0.1", 8089);
  tracethreat_controller_->send(&msgReq, msgResp);
	tracethreat_controller_->loop();
}
