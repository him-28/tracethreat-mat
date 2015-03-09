#ifndef INTERNET_TRACETHREAT_HPP
#define INTERNET_TRACETHREAT_HPP

#include "internet/tracethreat/infected_tracethreat.hpp"

namespace internet
{

    namespace tracethreat
    {


        CLASS_REGISTER_IMPLEMENT_REGISTRY(tracethreat_register, tracethreat);

        boost::scoped_ptr<tracethreat>& get_tracethreat()
        {
						static boost::scoped_ptr<tracethreat>  tracethreat;
            return tracethreat;

        }

        tracethreat *create_tracethreat()
        {
						tracethreat  * tracethreat = NULL;
            LOG(INFO)<<"Initial Tracethreat-Client-RPC module.";
            
            tracethreat = CREATE_TRACETHREAT("infected_tracethreat");

					  if(tracethreat == NULL){
                  LOG(INFO)<<" Mapper cannot create : tracethreat ";
            }
						return tracethreat;
        }


    }//rpc


}//internet




#endif /* INTERNET_TRACETHREAT_HPP */
