/*                       Titles                                          Authors                        Date
 * Add rule files to hnmav prog                                          R.Chatsiri
 *
 */
#include "wrapper/wrapper_rules.hpp"

TEST(wrapper_rule_compiles, wrapper_yr_rules_load)
{
	 using namespace wrapper;
	 char const * file_name = "/home/chatsiri/workspacecpp/pthread_sync_ocl/src/database/sample.rules";
	 
	 wrapper_rule_compiles<struct YARA_wrapper>  wrapper_rc;
	 EXPECT_EQ(wrapper_rc.wrapper_yr_rules_load(file_name, wrapper_rc.get_rules()), true);
};


