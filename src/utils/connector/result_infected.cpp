
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

            return true;
        }

				template class result_pe_infected<scan_threat::InfectedFileInfo>;

    }// connector

} //utils

