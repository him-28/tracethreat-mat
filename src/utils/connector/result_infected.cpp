#include "utils/connect/result_infected.hpp"

namespace utils
{

    namespace connector
    {

        template<typename Signature, typename Infected, typename FileStructure>
        class reslut_infected::impl
        {
            public:
                impl() : result_pe_inf(NULL) {


                }


                ~impl() {
                    delete result_pe_inf;
                }

                result_pe_infected<Signature, Infected, FileStructure> get_result_pe_inf() {
                    if(!result_pe_inf) {
                        result_pe_inf = new result_pe_infected<Signature, Infected, FileStructure>();
                    }

                    return result_pe_inf;
                }

            private:
                result_pe_infected<Signature, Infected, FileStructure>   *result_pe_inf;

        };

        template<typename Signature, typename Infected, typename FileStructure>
        result_infected::result_infected(): m_impl(new result_infected::impl)
        {


        }

				template<typename Signature, typename Infected, typename FileStructure>
        result_infected::~result_infected()
        {
							delete m_impl;
        }



    }

}

