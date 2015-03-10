#ifndef INTERNET_TRACETHREAT_TRACETHREAT_HPP
#define INTERNET_TRACETHREAT_TRACETHREAT_HPP

#include "utils/base/class_register.hpp"
#include "internet/tracethreat/infected_controller.hpp"

namespace internet
{

    namespace tracethreat
    {

        //template<typename MessageRequestType, typename MessageResponseType>  
        class tracethreat_controller;

        class tracethreat
        {

            public:

                typedef scan_threat::InfectedFileInfoRequest MessageRequestType;
                typedef scan_threat::InfectedFileInfoResponse MessageResponseType;

                virtual bool initial_engine(std::string dist_ip, int port) = 0;

                virtual bool send(MessageRequestType * msgReq, MessageResponseType * msgResp) = 0;

								virtual void received(MessageResponseType&   msgs) = 0;

                virtual void loop() = 0;

                virtual void break_loop() = 0;

                virtual internet::tracethreat::
                tracethreat_controller *
                get_tracethreat() = 0;

                ~tracethreat() { }

        };//tracethreat


        //System load crypto controller
        CLASS_REGISTER_DEFINE_REGISTRY(tracethreat_register, tracethreat);

        //Define macro of CLASS_REGISTER_OBJECT_CREATOR, CLASS_REGISTER_CREATE_OBJECT
#define REGISTER_TRACETHREAT(tracethreat_name) \
                      CLASS_REGISTER_OBJECT_CREATOR( \
                      tracethreat_register, \
                      tracethreat, \
                      #tracethreat_name, \
                      tracethreat_name) \
 
#define CREATE_TRACETHREAT(tracethreat_name_as_string) \
                      CLASS_REGISTER_CREATE_OBJECT(tracethreat_register, \
													tracethreat_name_as_string);




    }//tracethreat

}//internet

#endif /* INTERNET_TRACETHREAT_TRACETHREAT_HPP */
