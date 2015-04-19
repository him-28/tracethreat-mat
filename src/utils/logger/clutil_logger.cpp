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
 *
 */


#include "logger/clutil_logger.hpp"

namespace  utils
{

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::add_logger(CONT const&  logger_file)
    {
        logger_file_path.push_back(logger_file);
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::add_settings(CONT const& logger_settings_file)
    {
        logger_settings_path = logger_settings_file;
    }

    template<typename CONT, typename CONT1>
    shared_ptr<clutil_logging<CONT, CONT1> >& clutil_logging<CONT, CONT1>::get_instance()
    {

        if(logging_instance == NULL) {
            std::cout<<" Logging not get instance , It's create new instance " <<std::endl;
            logging_instance = new shared_ptr<clutil_logging<CONT, CONT1> >(
                    new clutil_logging<CONT, CONT1>()
            );
        }

        return  *logging_instance;

    }

    template<typename CONT, typename CONT1>
    clutil_logging<CONT, CONT1>::clutil_logging()
    {
        //backend
    }


    template<typename CONT, typename CONT1>
    clutil_logging<CONT, CONT1>::~clutil_logging()
    {

    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_info)
    {
        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_normal");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::normal) << write_info;


    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_detail, CONT const& write_info)
    {
        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_normal");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::normal) << write_detail << " : " << write_info;
    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_info, format_type::type type)
    {
        format_logger<std::string,int>  format_info(write_info, type);

        std::vector<std::string> *list_info = format_info.get_str_format();

        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_normal");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::normal) <<  &list_info[0];

    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_header,
            CONT const& write_detail,
            format_type::type type)
    {
        // Header
        format_logger<std::string,int>  format_log_header(write_header, type);

        std::vector<std::string> *list_header = format_log_header.get_str_format();

        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_normal");

				src::severity_logger<> lg = global_lg::get();

        BOOST_LOG_SEV(lg, severity_level::normal) << &list_header[0];

        // Info
        format_logger<std::string,int>  format_log_detail(write_detail, type);

        std::vector<std::string> *list_detail = format_log_detail.get_str_format();

        BOOST_LOG_SEV(lg, severity_level::normal) << &list_detail[0];

    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(char const& write_info)
    {
        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_normal");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::normal) << write_info;
    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info_test(CONT  const& write_detail, CONT const& write_info)
    {
        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_normal");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::normal) << "[TEST]: " << write_detail << " : " << write_info;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info_test(CONT const& write_info)
    {
        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_normal");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::normal) << "[TEST]: " << write_info;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_warning(CONT const& write_warning)
    {
        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_wraning");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::warning) << write_warning;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_error(CONT const& write_error)
    {
        BOOST_LOG_SCOPED_THREAD_TAG("SeverityID", "tracethreattools_error");
        src::severity_logger<> lg = global_lg::get();
        BOOST_LOG_SEV(lg, severity_level::error) << write_error;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::init_backend(severity_level sev_level)
    {

        sinks_tos_backend = boost::make_shared<sinks::text_multifile_backend>();


        sinks_tos_backend->set_file_name_composer
        (
                sinks::file::as_file_name_composer(expr::stream
                        << logger_file_path.back()
                        << expr::attr< std::string >("SeverityID")
                        << ".log")
        );

        boost::shared_ptr<sink_type> sink_(new sink_type(sinks_tos_backend));

        sink_->set_formatter(
                expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ", " << expr::attr< unsigned int >("RecordID")
                << ", " <<expr::smessage

        );

        switch(sev_level) {

        case severity_level::normal :
            sink_->set_filter(expr::attr<int>("Severity") == severity_level::normal);
            break;

        case severity_level::warning :
            sink_->set_filter(expr::attr<int>("Severity") == severity_level::warning);

            break;

        case severity_level::error :
            sink_->set_filter(expr::attr<int>("Severity") == severity_level::error);

            break;

        default:
            std::cout<<"Severity Log not supported.";
            break;
        }

        logging::core::get()->add_sink(sink_);


        logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
        logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());

    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::init_frontend()
    {



    }


    template class clutil_logging<std::string, int>;

}


