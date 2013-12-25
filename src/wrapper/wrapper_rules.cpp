
/*                       Titles                                          Authors                        Date
 * - Wrapper rules.c                                                     R.Chatsiri
 */

#include "wrapper/wrapper_rules.hpp"
#include "boost/lexical_cast.hpp"
namespace wrapper
{


    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::compile_rule(char **filename)
    {

    }


    template<typename Compiler>
    wrapper_rule_compiles<Compiler>::wrapper_rule_compiles()
    {

        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();
        logger->write_info("Wrapper-lib, Start... ", h_util::format_type::type_header);

    }

    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_rules_load(
            const char *filename,
            typename Compiler::rules_wrapper *rules)
    {
        int result;
        result = yr_rules_load(filename, &rules);
				logger->write_info("--Wrapper rules load, result : ", boost::lexical_cast<std::string>(result));
        if(result == ERROR_UNSUPPORTED_FILE_VERSION ||
                result == ERROR_CORRUPT_FILE) {
            return false;
        }

        return true;

    }

		template<typename Compiler>
		bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_create(typename Compiler::compiler_wrapper * compiler)
		{
				if(yr_compiler_create(&compiler) != ERROR_SUCCESS)
				{
						return false;
				}
			return true;
		}


    template class wrapper_rule_compiles<YARA_wrapper>;

}
