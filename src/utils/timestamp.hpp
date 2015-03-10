#ifndef UTILS_TIMESTAMP_HPP
#define UTILS_TIMESTAMP_HPP

#include <boost/date_time/local_time/local_time.hpp>

#include <sstream>
#include <string>

namespace utils{

   class timestamp{
			public:

					timestamp();
		
					std::string initial_timezone();

					std::string get_system_time();

			private:
					boost::posix_time::ptime  now;
          boost::posix_time::ptime  utc;
          boost::local_time::local_time_facet* output_facet;
          std::stringstream   ss;
	 };

}


#endif /* UTILS_TIMESTAMP_HPP */
