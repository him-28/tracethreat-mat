/*
* Copyright 2014 MTSec, Inc.
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

namespace  hnmav_util
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
            logging_instance = new shared_ptr<clutil_logging<CONT, CONT1> >(new clutil_logging<CONT, CONT1>());
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
        for(std::vector<shared_ptr<sinks::synchronous_sink<
                sinks::text_multifile_backend> >
                >::iterator iter = sinks_vec.begin();
                iter != sinks_vec.end();
                ++iter) {
            shared_ptr<sinks::synchronous_sink< sinks::text_multifile_backend> > syn_sinks_ptr = *iter;
            syn_sinks_ptr.reset();
        }
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_info)
    {
        //BOOST_LOG(write_log::get())<< write_info;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_detail, CONT const& write_info)
    {
        //BOOST_LOG(write_log::get())<< write_detail << " : " << write_info;
    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_info, format_type::type type)
    {
        format_logger<std::string,int>  format_info(write_info, type);

        std::vector<std::string> *list_info = format_info.get_str_format();

        for(std::vector<std::string>::iterator iter_log = list_info->begin(); iter_log !=  list_info->end() ; ++iter_log)
            BOOST_LOG(write_log::get())<< *iter_log;
    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(CONT  const& write_header,
            CONT const& write_detail,
            format_type::type type)
    {
        // Header
        format_logger<std::string,int>  format_log_header(write_header, type);

        std::vector<std::string> *list_header = format_log_header.get_str_format();

        for(std::vector<std::string>::iterator iter_log = list_header->begin(); iter_log !=  list_header->end() ; ++iter_log)
            BOOST_LOG(write_log::get())<< *iter_log;

        // Info
        format_logger<std::string,int>  format_log_detail(write_detail, type);

        std::vector<std::string> *list_detail = format_log_detail.get_str_format();

        for(std::vector<std::string>::iterator iter_log = list_detail->begin(); iter_log !=  list_detail->end() ; ++iter_log)
            BOOST_LOG(write_log::get())<< *iter_log;
    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info(char const& write_info)
    {
        //BOOST_LOG(write_log::get()) << write_info;
    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info_test(CONT  const& write_detail, CONT const& write_info)
    {
        //BOOST_LOG(write_log::get())<< "[TEST]: " << write_detail << " : " << write_info;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_info_test(CONT const& write_info)
    {
        //BOOST_LOG(write_log::get()) << "[TEST]: " << write_info;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_warning(CONT const& write_warning)
    {
        BOOST_LOG(write_log::get())<< write_warning;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::write_error(CONT const& write_error)
    {
        BOOST_LOG(write_log::get())<< write_error;
    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::formatter_normal()
    {
        file_sink_ptr sinks_ptr = sinks_vec.back();

        sinks_ptr->locked_backend()->set_formatter(
                fmt::format("%1%:- [%2%] :- %3% ")
                % fmt::attr< unsigned int >("Line #")
                % fmt::date_time< boost::posix_time::ptime >("TimeStamp")
                % fmt::message()
        );

    }
    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::formatter_error()
    {
        file_sink_ptr sinks_ptr = sinks_vec.back();

        sinks_ptr->locked_backend()->set_formatter(
                fmt::format("%1%:- [%2%] Error :- %3% ")
                % fmt::attr< unsigned int >("Line #")
                % fmt::date_time< boost::posix_time::ptime >("TimeStamp")
                % fmt::message()
        );

    }


    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::init_backend()
    {
        std::cout<<" Init logging backend()..." <<std::endl;
        /*
        				std::ifstream file_settings(logger_settings_path.c_str());
        				if(!file_settings)
        				{
        					std::cout<< "cannot open file = " << logger_settings_path<<", using default configure" <<std::endl;

        				}
        				else
        				{
        					logging::init_from_stream(file_settings);
        				}
        */
        file_sink_ptr sinks_ptr  = shared_ptr<sinks::synchronous_sink
                < sinks::text_multifile_backend> >(new file_sink);

        for(std::vector<std::string>::iterator  iter = logger_file_path.begin();
                iter != logger_file_path.end();
                ++iter) {
            sinks_ptr->locked_backend()->set_file_name_composer(
                    fmt::stream<< *iter << ".log");
            std::cout<<" logger path = " << *iter <<std::endl;
        }

        sinks_vec.push_back(sinks_ptr);


    }

    template<typename CONT, typename CONT1>
    void clutil_logging<CONT, CONT1>::init_frontend()
    {
        std::cout<<" Init logging frontend()..." <<std::endl;
        core_ptr core = logging::core::get();
        core->get()->add_sink(sinks_vec.back());

        // Add some attributes too
        shared_ptr< logging::attribute > attr(new attrs::local_clock);
        core->get()->add_global_attribute("TimeStamp", attr);
        attr.reset(new attrs::counter< unsigned int >);
        core->get()->add_global_attribute("Line #", attr);


        frontend_core_vec.push_back(core);
    }

		template class clutil_logging<std::string, int>;

}


