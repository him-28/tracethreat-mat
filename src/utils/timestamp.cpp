#include "utils/timestamp.hpp"

namespace utils
{

    timestamp::timestamp()
    {
				
    }

    std::string timestamp::initial_timezone()
    {

				return "";
    }

    std::string timestamp::get_system_time()
    {

        now = boost::posix_time::second_clock::local_time();
        utc = boost::posix_time::second_clock::universal_time();

        boost::posix_time::time_duration tz_offset = (now - utc);

        output_facet = new boost::local_time::local_time_facet();

        ss.imbue(std::locale(std::locale::classic(), output_facet));

        output_facet->format("%H:%M:%S");

        ss.str("");

        ss << tz_offset;

        boost::local_time::time_zone_ptr    zone(new boost::local_time::posix_time_zone(ss.str().c_str()));

        boost::local_time::local_date_time  ldt = boost::local_time::local_microsec_clock::local_time(zone);

        output_facet = new boost::local_time::local_time_facet();

        ss.imbue(std::locale(std::locale::classic(), output_facet));

        output_facet->format("%Y-%m-%d %H:%M:%S%f%Q");
        ss.str("");
        ss << ldt;
        return ss.str();
    }

}

