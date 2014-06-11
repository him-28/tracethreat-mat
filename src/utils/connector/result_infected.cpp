
#include "utils/connector/result_infected.hpp"

#include "utils/base/system_code.hpp"

namespace utils
{

    namespace connector
    {

        template<typename Signature, typename FileStructure>
        class result_infected<Signature, FileStructure>::impl
        {
            public:
                impl() : result_pe_inf(NULL) {


                }

                ~impl() {
                    delete result_pe_inf;
                }

                result_pe_infected<Signature, FileStructure> *get_result_pe_inf() {
                    if(!result_pe_inf) {
                        result_pe_inf = new result_pe_infected<Signature, FileStructure>();
                    }

                    return result_pe_inf;
                }

                bool add_infected_result(Signature *msig) {
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
                }

            private:
                result_pe_infected<Signature, FileStructure>   *result_pe_inf;

        };

        template<typename Signature, typename FileStructure>
        result_infected<Signature, FileStructure>::result_infected():
            m_impl(new result_infected<Signature, FileStructure>::impl)
        {


        }

        template<typename Signature, typename FileStructure>
        result_infected<Signature, FileStructure>::~result_infected()
        {
            delete m_impl;
        }

				//Not support implement internal class. ::impl::add_infected_result
        /*
        template<typename Signature, typename FileStructure>
        bool result_infected<Signature, FileStructure>::impl::add_infected_result(Signature *msig)
        {
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
        }
        */

        //__________________________   result_pe_infected ____________________________________
        template<typename Signature, typename FileStructure>
        bool result_pe_infected<Signature, FileStructure>::add_infected_result(Signature *msig)
        {
            //Case for null data not data on structure of meta_sig;
            if(msig->sig_type  == 0)
                return false;

            sig_map.insert(*msig->sig, msig);

            logger->write_info("result_pe_infected::add_infected_result, size name insert",
                    boost::lexical_cast<std::string>(msig->sig));

            logger->write_info("result_pe_infected::add_infected_result, size of map after insert",
                    boost::lexical_cast<std::string>(sig_map.size()));
            return true;
        }



    }// connector

} //utils

