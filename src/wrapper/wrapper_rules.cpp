
/*                       Titles                                          Authors                        Date
 * - Wrapper rules.c                                                     R.Chatsiri
 */

#include "wrapper/wrapper_rules.hpp"
namespace wrapper{

template<typename YARA_wrapper>
bool wrapper_rule_compiles<YARA_wrapper>::wrapper_yr_rules_load(
        const char *filename,
        typename YARA_wrapper::rules_wrapper * rules)
{
		int result;
		result = yr_rules_load(filename, &rules);
		if(result == ERROR_UNSUPPORTED_FILE_VERSION ||
			 result == ERROR_CORRUPT_FILE)
		{
				return false;
		}
		return true;
		
}



template<typename YARA_Rules>
bool wrapper_rule_compiles<YARA_Rules>::compile_rule(char ** filename)
{

}

template class wrapper_rule_compiles<YARA_wrapper>;

}
