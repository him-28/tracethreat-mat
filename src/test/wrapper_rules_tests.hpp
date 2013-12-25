/*                       Titles                                          Authors                        Date
 * Add rule files to hnmav prog                                          R.Chatsiri
 *
 */
#include "wrapper/wrapper_rules.hpp"
using namespace wrapper;

class WrapperRuleCompilerTest : public ::testing::Test
{
	protected:
	 virtual void SetUp(){

	  argv[0] = "/home/chatsiri/sda1/workspacemalware/yara/build/bin/yara";
	  argv[1] = "-r";
    argv[2] = "/home/chatsiri/Dropbox/reversing_engineer/malwarecookbook/3/3/clamav.yara";
    argv[3] = "/home/chatsiri/sda1/workspacecpp/clamav-devel/test/clam.exe"; 

	 }
		
	  wrapper_rule_compiles<struct YARA_wrapper>  wrapper_rc;
	  char const * argv[4];

};

TEST_F(WrapperRuleCompilerTest, wrapper_yr_rules_load)
{
	   	 
		 EXPECT_EQ(wrapper_rc.wrapper_yr_rules_load(argv[2], wrapper_rc.get_rules()), true);

}

TEST_F(WrapperRuleCompilerTest, wrapper_yr_compiler_create)
{
   EXPECT_EQ(wrapper_rc.wrapper_yr_compiler_create(&wrapper_rc.get_compiler()), true);

}

