/*                       Titles                                          Authors                        Date
 * Add rule files to hnmav prog                                          R.Chatsiri
 *
 */
#include "wrapper/wrapper_rules.hpp"

TEST(wrapper_rule_compiles, wrapper_yr_rules_load)
{
	 using namespace wrapper;
	 char const * file_name = "test.cpp";
	 struct YARA_Rules yr_rules;
	 
	 wrapper_rule_compiles<struct YARA_Rules>  wrapper_rc;

	 iwrapper<struct YARA_Rules> *  wrapper = &wrapper_rc;
};


