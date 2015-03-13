
#include "utils/connector/result_infected.hpp"

#include "utils/base/system_code.hpp"

namespace utils
{

    namespace connector
    {

        template<typename MessageResult>
        class result_infected<MessageResult>::impl
        {
            public:
                impl() : result_pe_inf(NULL) {


                }

                ~impl() {
                    delete result_pe_inf;
                }

                result_pe_infected<MessageResult> *get_result_pe_inf() {
                    if(!result_pe_inf) {
                        result_pe_inf = new result_pe_infected<MessageResult>();
                    }

                    return result_pe_inf;
                }

                bool add_infected_result(MessageResult *msig) {
										/*
                    switch(msig->sig_type) {

                    case utils::pe_file :
                        //Meta signature of PE Infomation.
                        result_pe_inf->add_infected_result(msig);

                    case utils::elf_file :
                        break;

                    default:
                        logger->write_info("!!! File not support end result container",
                                boost::lexical_cast<std::string>(msig->sig_type));
                        logger->write_info("!!! File Name Detail",
                                boost::lexical_cast<std::string>(msig->sig_detail));
                    }
										*/
                }

            private:
                result_pe_infected<MessageResult>   *result_pe_inf;

        };

        template<typename MessageResult>
        result_infected<MessageResult>::result_infected():
            m_impl(new result_infected<MessageResult>::impl)
        {


        }

        template<typename MessageResult>
        result_infected<MessageResult>::~result_infected()
        {
            delete m_impl;
        }
			
			template class result_infected<scan_threat::InfectedFileInfo>;

        //__________________________   result_pe_infected ____________________________________
        template<typename MessageResult>
        bool result_pe_infected<MessageResult>::add_infected_result(MessageResult *msig)
        {
            //Case for null data not data on structure of meta_sig;
            /*
            if(msig->sig_type  == 0)
                return false;
		
            sig_map.insert(*msig->sig, msig);
						
            logger->write_info("result_pe_infected::add_infected_result, size name insert",
                    boost::lexical_cast<std::string>(msig->sig));

            logger->write_info("result_pe_infected::add_infected_result, size of map after insert",
                    boost::lexical_cast<std::string>(sig_map.size()));
            */
            return true;
        }

				template class result_pe_infected<scan_threat::InfectedFileInfo>;

    }// connector

} //utils

