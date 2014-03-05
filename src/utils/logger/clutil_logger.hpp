#ifndef HNMAV_UTIL_CLUTIL_LOGGER_HPP
#define HNMAV_UTIL_CLUTIL_LOGGER_HPP

/*-Titles                                                   Authors                 Date
 *-Init Class logging for project ( Logging version not includes log level.)
 *                                                          Chatsiri.rat            26/02/2012
 *-Add shared ptr for handling log	( unthreadsafe )        Cahtsiri.rat            04/10/2012
 */

//BOOST
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
//log common
#include <boost/log/common.hpp>
#include <boost/log/formatters.hpp>
#include <boost/log/filters.hpp>
#include <boost/log/attributes/timer.hpp>
//log util
#include <boost/log/utility/init/to_file.hpp>
#include <boost/log/utility/init/to_console.hpp>
#include <boost/log/utility/init/common_attributes.hpp>
#include <boost/log/utility/init/from_stream.hpp>
//log sinks
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
//thread,time
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
//STL
#include <iostream>
#include <string>
#include <sstream>
//INTERNAL
#include "format_logger.hpp"

namespace  hnmav_util
{
    namespace logging = boost::log;
    namespace fmt			= boost::log::formatters;
    namespace flt 		= boost::log::filters;
    namespace sinks		= boost::log::sinks;
    namespace attrs		= boost::log::attributes;
    namespace src			= boost::log::sources;
    namespace keywords = boost::log::keywords;
    using namespace boost;

    enum severity_level {
        normal,
        warning,
        error
    };


    BOOST_LOG_DECLARE_GLOBAL_LOGGER(write_log,src::logger_mt)

    template<typename CONT = std::string,  typename CONT1 =  int  >
    class clutil_logging
    {
        public:
            void init_backend();
            void init_frontend();

            void formatter_normal();
            void formatter_error();

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

            clutil_logging();
            clutil_logging& operator=(const clutil_logging& logging);
            clutil_logging(const clutil_logging& logging);

            // back end
            typedef sinks::synchronous_sink< sinks::text_multifile_backend >  file_sink;
            typedef shared_ptr<sinks::synchronous_sink< sinks::text_multifile_backend> >  file_sink_ptr;
            std::vector<shared_ptr<sinks::synchronous_sink< sinks::text_multifile_backend> > >  sinks_vec;

            // front end
            typedef shared_ptr<logging::core>   core_ptr;
            std::vector<shared_ptr<logging::core> > frontend_core_vec;

            std::vector<std::string> logger_file_path;
            std::string logger_settings_path;

    };

    template<typename CONT, typename CONT1>
    shared_ptr<clutil_logging<CONT, CONT1> > *clutil_logging<CONT, CONT1>::logging_instance = NULL;

}

#endif /* HNMAV_UTIL_CLUTIL_LOGGER_HPP */



