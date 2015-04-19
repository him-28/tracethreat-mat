#ifndef HNMAV_UTIL_OPTIONS_SYSTEM_HPP
#define HNMAV_UTIL_OPTIONS_SYSTEM_HPP

/*
* Copyright 2014 Chatsiri Rattana.
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

/*  Titles																								Authors					 Date
 *-Add commandline option                                   Chatsiri.rat            14/09/2012
 *-Un thread safe                                           Chatsiri.rat            14/09/2012
 *-Add shared_ptr                                           Chatsiri.rat            20/09/2012
 *-Add exception base class system                          Chatsiri.rat            18/11/2012
 *-Add Not new instance.                                    Chatsiri.rat            21/11/2012
 *
 */

//BOOST
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
// STD
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>


// internal
#include <exception/system_exception.hpp>

namespace utils
{

    namespace po = boost::program_options;
    namespace exp_system = trace_exception::system;

    using namespace boost;

    class options_system
    {
        public:

            static options_system& get_instance();

            void generic_options();
            int default_condition();

            void configure_options();

            void process_options(int ac, char *av[]);

						//signature & logging path.
            std::string  get_scanfile_path()const;
            std::string  get_database_path()const;
            std::string  get_openclfile_path() const;
            std::string  get_logger_mainfile_path() const;
            std::string  get_logger_settingsfile_path() const;
            std::string&  get_config_file()const;

						//security configure.
						std::string get_certssl_path()const;
						std::string get_dh512_path()const;
						std::string get_pwd()const;

						//server ip, port
						std::string get_ip_server()const;
						std::string get_port_server()const;

						//tracethreat ip,port
						std::string get_ip_tracethreat()const;
						std::string get_port_tracethreat()const;

        private:
            options_system();
            ~options_system();
            options_system(const options_system&);
            const options_system& operator=(const options_system&);

            void  read_config(std::stringstream&   config_type, std::vector<std::string>&   config_name);

						//system path
            std::stringstream  *scanfile_path;
            std::stringstream  *database_path;
            std::stringstream  *clfile_path;
            std::stringstream  *logger_mainfile_path;
            std::stringstream  *logger_settingsfile_path;

						//security path
						std::stringstream  *cert_path;
						std::stringstream  *dh512_path;
            std::stringstream  *certpwd;

						//server ip, port
						std::stringstream *ip_addr_server;
						std::stringstream *port_server;

						//tracethreat
						std::stringstream *ip_tracethreat;
						std::stringstream *port_tracethreat;

						//configure file path
            std::string *config_file;

            po::variables_map vm;

            po::options_description   *visible;
            po::options_description   *generics;
            po::options_description   *configure;
            po::options_description   *config_file_options;

            typedef shared_ptr<po::options_description>   options_visible;
            typedef shared_ptr<po::options_description>   options_generics;
            typedef shared_ptr<po::options_description>   options_configure;
            typedef shared_ptr<po::options_description>   options_configfile;

            std::vector<shared_ptr<po::options_description> >  options_type;

		        static options_system * options_system_instance; 				
    };
 
}

#endif /* HNMAV_UTIL_OPTIONAL_SYSTEM_HPP */

