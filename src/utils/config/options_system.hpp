#ifndef HNMAV_UTIL_OPTIONS_SYSTEM_HPP
#define HNMAV_UTIL_OPTIONS_SYSTEM_HPP

/*-Titles                                                   Authors                 Date
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

//#include "utils/config/options_system.cpp"

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

            std::string  get_scanfile_path()const;
            std::string  get_database_path()const;
            std::string  get_openclfile_path() const;
            std::string  get_logger_mainfile_path() const;
            std::string  get_logger_settingsfile_path() const;
            std::string&  get_config_file()const;
						//std::string& get_dir_path()const;

        private:
            options_system();
            ~options_system();
            options_system(const options_system&);
            const options_system& operator=(const options_system&);

            void  read_config(std::stringstream&   config_type, std::vector<std::string>&   config_name);

            std::stringstream  *scanfile_path;
            std::stringstream  *database_path;
            std::stringstream  *clfile_path;
            std::stringstream  *logger_mainfile_path;
            std::stringstream  *logger_settingsfile_path;

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

