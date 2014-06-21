#ifndef UTILS_RESULT_INFECTED_HPP
#define UTILS_RESULT_INFECTED_HPP

#include <boost/unordered_map.hpp>


#include "utils/logger/clutil_logger.hpp"

#include "utils/base/common.hpp"
#include "memory/signature_shm_base.hpp"

namespace utils
{

    namespace connector
    {
				namespace h_util = hnmav_util;
        //struct meta_sig, class file structure.
        template<typename Signature, typename FileStructure>
        class result_infected
        {

            public:

                result_infected();
                ~result_infected();

            private:

                result_infected(const result_infected&);

                const result_infected& operator=(const result_infected&);

                //Add signature type and structure meta_sig.
                //support utils::xxx_type such utils::pe_file
                bool add_infected_result(Signature *msig);

                Signature *sig;

                class impl;
                impl *m_impl;

                //logger
                boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
                h_util::clutil_logging<std::string, int>    *logger;


        };

        template<typename Signature, typename FileStructure>
        class result_pe_infected
        {
                typedef boost::unordered_map<char, Signature *> sigtype_map;
            public:
                bool add_infected_result(Signature *msig);
                bool list_index_found(Signature *sig_, FileStructure   *file_str_);

            private:
                sigtype_map sig_map;

                //logger
                boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
                h_util::clutil_logging<std::string, int>    *logger;

        };


    } // Connector

} //Utils

#endif /* UTILS_RESULT_INFECTED_HPP */