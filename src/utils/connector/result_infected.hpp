#ifndef UTILS_RESULT_INFECTED_HPP
#define UTILS_RESULT_INFECTED_HPP

#include "utils/base/common.hpp"
namespace utils
{

    namespace connector
    {

        template<typename Signature, typename Infected, typename FileStructure>
        class result_infected
        {

            public:

                result_infected();
                ~result_infected();

                bool check_matched(Signature *sig_, Infected *infected_);

            private:
                result_infected(const result_infected&);
                const result_infected& operator=(const result_infected&);

                Signature *sig;
                Infected   *infected_file;

                class impl;
                impl *m_impl;

        };

        template<typename Signature, typename Infected, typename FileStructure>
        class result_pe_infected
        {

            public:

                bool list_index_found(Signature *sig_, Infected *infected_, FileStructure   *file_str_);

            private:

        };


    }

}

#endif /* UTILS_RESULT_INFECTED_HPP */
