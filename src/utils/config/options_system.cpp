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
 *
 */



#include "utils/config/options_system.hpp"

namespace utils
{

    options_system *options_system::options_system_instance = NULL;

    void options_system::read_config(std::stringstream& config_type, std::vector<std::string>& configname_vec)
    {
        std::copy(configname_vec.begin(),
                configname_vec.end(),
                std::ostream_iterator<std::string>(config_type," "));
    }


    options_system::options_system()
    {
        configure = new po::options_description("Configuration");
        options_configure  configure_ptr(configure);
        options_type.push_back(configure_ptr);

        visible  = new po::options_description("Allowed options");
        options_visible visible_ptr(visible);
        options_type.push_back(visible_ptr);

        generics = new po::options_description("Generic options");
        options_generics  generics_ptr(generics);
        options_type.push_back(generics_ptr);

    }

    options_system::~options_system()
    {
        for(std::vector<shared_ptr<po::options_description> >::iterator iter = options_type.begin();
                iter != options_type.end();
                ++iter) {
            shared_ptr<po::options_description> options_ptr = *iter;
            options_ptr.reset();
        }

    }
    // opencl file standard
    std::string options_system::get_openclfile_path() const
    {
        std::string clpath = clfile_path->str();
        boost::trim(clpath);
        return clpath;
    }
    // scan file specific path
    std::string options_system::get_scanfile_path() const
    {
        std::string scanpath = scanfile_path->str();
        boost::trim(scanpath);
        return scanpath;
    }
    // database signature virus path
    std::string options_system::get_database_path() const
    {
        std::string dbpath = database_path->str();
        boost::trim(dbpath);
        return dbpath;
    }


		//Security file path.
    std::string options_system::get_certssl_path()const
    {
        std::string certpath = cert_path->str();
        boost::trim(certpath);
        return certpath;
    }

    std::string options_system::get_dh512_path()const
    {
        std::string dh512 = dh512_path->str();
        boost::trim(dh512);
        return dh512;
    }

    std::string options_system::get_pwd()const
    {
        std::string pwd = certpwd->str();
        boost::trim(pwd);
        return pwd;
    }


		//Server IP and Port
		std::string options_system::get_ip_server()const{
			std::string ip = ip_addr_server->str();
			boost::trim(ip);
			return ip; 
		}

		std::string options_system::get_port_server()const{
			std::string port = port_server->str();
			boost::trim(port);
			return port;
		}


		//Tracethreat IP and Port
		std::string options_system::get_ip_tracethreat()const{
			std::string ip_tt = ip_tracethreat->str();
			boost::trim(ip_tt);
			return ip_tt;
		}

		std::string options_system::get_port_tracethreat()const{
				std::string port_tt = port_tracethreat->str();
				boost::trim(port_tt);
				return port_tt;
		}


    // logger main file path
    std::string options_system::get_logger_mainfile_path() const
    {
        std::string logger_main;
        //default name of main logger
        std::string default_file_name = "logger_main";


        try {
            if(logger_mainfile_path == NULL)
                throw  exp_system::option_system_exception(default_file_name);
            else
                logger_main = logger_mainfile_path->str();
        } catch(exp_system::option_system_exception& os_exception) {

            std::string error_detail;
            tie(error_detail, logger_main) = os_exception.message_error_path();
        }

        boost::trim(logger_main);
        return logger_main;
    }

    // logger setting file
    std::string options_system::get_logger_settingsfile_path()const
    {
        std::string logger_settings;
        //default name of setting logger
        std::string default_file_setting_name = "/logger/setting.txt";

        try {

            if(logger_settingsfile_path == NULL)
                throw exp_system::option_system_exception(default_file_setting_name);
            else
                logger_settings = logger_settingsfile_path->str();
        } catch(exp_system::option_system_exception& os_exception) {

            std::string error_detail;
            tie(error_detail, logger_settings) = os_exception.message_error_path();
        }

        boost::trim(logger_settings);
        return logger_settings;
    }

    options_system&  options_system::get_instance()
    {
        if(options_system_instance == NULL) {
            options_system_instance = new options_system();
        }

        return *options_system_instance;
    }


    void  options_system::generic_options()
    {
        try {
            config_file = new std::string;
            generics->add_options()
            ("version,v", " print version string")
            ("help", " help options of programs")
            ("config,c", po::value<std::string>(config_file)->default_value("resource.cfg"),
                    " name of a file of a configuration.");
        } catch(std::exception ex) {
            std::cout<< "Error : " <<  ex.what() <<std::endl;
        }
    }

    void options_system::configure_options()
    {
        try {

            configure->add_options()
            ("db-signature-path,d",
                    po::value< std::vector<std::string> >()->composing(),
                    " database signature path")
            ("scanning-file,f",
                    po::value< std::vector<std::string> >()->composing(),
                    " scanning file path")
            ("cl-file,o",
                    po::value< std::vector<std::string> >()->composing(),
                    " kernel cl file path")
            ("logger-main,l",
                    po::value< std::vector<std::string> >()->composing(),
                    " logger main file")
            ("logger-settings,s",
                    po::value< std::vector<std::string> >()->composing(),
                    " logger settings file")
            ("cert-pem,e",
                    po::value< std::vector<std::string> >()->composing(),
                    " ceritificate file of SSL")
            ("dh512-pem,d",
                    po::value< std::vector<std::string> >()->composing(),
                    " DH512-PEM of SSL")
            ("cert-pwd,p",
                    po::value< std::vector<std::string> >()->composing(),
                    " Password of SSL")
						("ip-addr-server",
                    po::value< std::vector<std::string> >()->composing(),
                    " Server local IP")
						("port-server",
                    po::value< std::vector<std::string> >()->composing(),
                    " Server local Port")
						("ip-tracethreat",
                    po::value< std::vector<std::string> >()->composing(),
                    " Tracethreat-DB IP")
						("port-tracethreat",
                    po::value< std::vector<std::string> >()->composing(),
                    " Tracethreat-DB Port");
	

						




        } catch(std::exception ex) {
            std::cout<< "Error : " << ex.what() <<std::endl;
        }
    }

    void options_system::process_options(int ac, char *av[])
    {
        try {
            po::options_description  cmdline_options;
            cmdline_options.add(*generics).add(*configure);

            visible->add(*generics);
            visible->add(*configure);

            store(po::command_line_parser(ac,av).options(cmdline_options).run(), vm);
            notify(vm);


        } catch(std::exception ex) {
            std::cout<<" Error : " <<  ex.what() <<std::endl;
        }

    }

    int options_system::default_condition()
    {
        std::ifstream ifs(config_file->c_str());

        if(!ifs) {
            std::cout<< "cannot open file name : " << *config_file <<std::endl;
        } else {

            config_file_options  = new po::options_description();
            config_file_options->add(*configure);

            store(parse_config_file(ifs, *config_file_options),vm);
            notify(vm);
        }



        if(vm.count("help")) {
            std::cout<< *visible <<std::endl;
            return 0;
        }

        if(vm.count("version")) {
            std::cout<< "Tracethreat, version releases :  0.0.1 " <<std::endl;
        }

        if(vm.count("db-signature-path")) {
            std::vector<std::string>  vec =  vm["db-signature-path"].as<std::vector<std::string> >();
            database_path = new std::stringstream;
            read_config(*database_path,vec);
        }

        if(vm.count("scanning-file")) {
            std::vector<std::string>  vec =  vm["scanning-file"].as<std::vector<std::string> >();
            scanfile_path = new std::stringstream;
            read_config(*scanfile_path,vec);
        }

        if(vm.count("cl-file")) {
            std::vector<std::string>  vec =  vm["cl-file"].as<std::vector<std::string> >();
            clfile_path = new std::stringstream;
            read_config(*clfile_path,vec);
        }

        if(vm.count("logger-main")) {
            std::vector<std::string>  vec =  vm["logger-main"].as<std::vector<std::string> >();
            logger_mainfile_path = new std::stringstream;
            read_config(*logger_mainfile_path,vec);
        }

        if(vm.count("logger-settings")) {
            std::vector<std::string> vec =  vm["logger-settings"].as<std::vector<std::string> >();
            logger_settingsfile_path = new std::stringstream;
            read_config(*logger_settingsfile_path,vec);

        }


        //support security path
        if(vm.count("cert-pem")) {
            std::vector<std::string> vec =  vm["cert-pem"].as<std::vector<std::string> >();
            cert_path = new std::stringstream;
            read_config(*cert_path,vec);

        }

        if(vm.count("dh512-pem")) {
            std::vector<std::string> vec =  vm["dh512-pem"].as<std::vector<std::string> >();
            dh512_path = new std::stringstream;
            read_config(*dh512_path,vec);
        }//dh515-pem

        if(vm.count("cert-pwd")) {
            std::vector<std::string> vec =  vm["cert-pwd"].as<std::vector<std::string> >();
            certpwd = new std::stringstream;
            read_config(*certpwd,vec);
        }//cert-pwd


        if(vm.count("ip-addr-server")) {
            std::vector<std::string> vec =  vm["ip-addr-server"].as<std::vector<std::string> >();
            ip_addr_server = new std::stringstream;
            read_config(*ip_addr_server,vec);
        }//ip-addr-server

        if(vm.count("port-server")) {
            std::vector<std::string> vec =  vm["port-server"].as<std::vector<std::string> >();
            port_server = new std::stringstream;
            read_config(*port_server, vec);
        }//port-server

        if(vm.count("ip-tracethreat")) {
            std::vector<std::string> vec =  vm["ip-tracethreat"].as<std::vector<std::string> >();
            ip_tracethreat = new std::stringstream;
            read_config(*ip_tracethreat, vec);
        }//ip-tracethreat

        if(vm.count("port-tracethreat")) {
            std::vector<std::string> vec =  vm["port-tracethreat"].as<std::vector<std::string> >();
            port_tracethreat = new std::stringstream;
            read_config(*port_tracethreat, vec);
        }//port-tracethreat


    }


}
