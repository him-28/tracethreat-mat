#ifndef UTILS_CLUTIL_LOGGER_HPP
#define UTILS_CLUTIL_LOGGER_HPP
#define BOOST_LOG_DYN_LINK

/*
* Copyright 2014 R.Chatsiri, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                                     Authors	          Date
 *-Init Class logging for project ( Logging version not includes log level.)
 *                                                               Chatsiri.rat     26/02/2012
 *-Add shared ptr for handling log	( unthreadsafe )             Chtasiri.rat     04/10/2012
 */

//STL
#include <ostream>
#include <string>

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
//log common
#include <boost/log/common.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/trivial.hpp>
//log util
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>

#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

//thread,time
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
//INTERNAL
#include "format_logger.hpp"

namespace  utils
{
    namespace logging = boost::log;
    //namespace fmt			= boost::log::formatters;
    //namespace flt 		= boost::log::filters;
    namespace sinks		= boost::log::sinks;
    namespace attrs		= boost::log::attributes;
    namespace src			= boost::log::sources;
    namespace keywords = boost::log::keywords;
    namespace expr = boost::log::expressions;
    using namespace boost;

    enum severity_level {
        normal,
        warning,
        error
    };


		BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(global_lg, src::severity_logger<>)
    //BOOST_LOG_DECLARE_GLOBAL_LOGGER(write_log,src::logger_mt)

    template<typename CONT = std::string,  typename CONT1 =  int  >
    class clutil_logging
    {
        public:

            typedef sinks::synchronous_sink< sinks::text_multifile_backend > sink_type;

            void init_backend(severity_level serv);
            void init_frontend();

            //void formatter_normal();
            void formatter_error();

            //void log_level(logging::trivial::severity & trivail_level);
            //bool only_warnings(const logging::attribute_value_set &set);

            void write_info(CONT const& write_info);
            void write_info(CONT const& write_info, format_type::type type);


            void write_info(CONT const& write_detail, CONT const& write_info);
            void write_info(CONT  const& write_detail, CONT const& write_info, format_type::type  type);
            void write_info(char const& write_info);

            void write_info_test(CONT const& write_detail, CONT const& write_info);
            void write_info_test(CONT const& write_info);


            void write_error(CONT const& write_error);
            void write_warning(CONT const& write_warning);

            static shared_ptr<clutil_logging<CONT, CONT1> >& get_instance();
            void add_logger(CONT const&);
            void add_settings(CONT const&);


            ~clutil_logging();


        private:
            CONT  file_path_;
            CONT1 log_level_;
            bool isInt_;

            static shared_ptr<clutil_logging<CONT, CONT1> >   *logging_instance;

            clutil_logging();// : lg(global_lg::get()){ }

            clutil_logging& operator=(const clutil_logging& logging);
            clutil_logging(const clutil_logging& logging);

            // back end
            boost::shared_ptr<sinks::text_multifile_backend> sinks_tos_backend;

            // front end
            typedef shared_ptr<logging::core>   core_ptr;
            std::vector<shared_ptr<logging::core> > frontend_core_vec;

            std::vector<std::string> logger_file_path;
            std::string logger_settings_path;

            //src::severity_logger<> & lg;

    };

    template<typename CONT, typename CONT1>
    shared_ptr<clutil_logging<CONT, CONT1> > *clutil_logging<CONT, CONT1>::logging_instance = NULL;

}

#endif /* HNMAV_UTIL_CLUTIL_LOGGER_HPP */



