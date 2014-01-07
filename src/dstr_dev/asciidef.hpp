#ifndef DATA_STRUCTURE_ASCIIDEF_HPP
#define DATA_STRUCTURE_ASCIIDEF_HPP

/*                       Titles                                          Authors                        Date
 *- Add ascii table and tag																							 R.Chatsiri											20/03/2013
 *
 */

#include "logger/clutil_logger.hpp"
#include "data_structure/absasciidef.hpp"

template<typename AsciiType_Tag>
class ascii_tags
{
        typedef asciihex_policy  ah_policy;
        typedef asciichar_policy ac_policy;
        typedef asciioct_policy  ao_policy;
};


template<typename AsciiTables_Tag>
class ascii_tables : public vector<AsciiTables_Tag>
{
    public:
        ascii_tables();
        char *build_tables();
        char *find(char const& input);
				Iterator<AsciiTables_Tag> absiter();
    private:
        char *assics;
};

template<typename AsciiTables_Tag>
ascii_tables<AsciiTables_Tag>::ascii_tables()
{
    util::options_system& op_system = util::options_system::get_instance();
    //init logger
    logger_ptr = &util::clutil_logging<std::string, int>::get_instance();
    logger = logger_ptr->get();
}


#endif /* DATA_STRUCTURE_ASCIIDEF_HPP */
