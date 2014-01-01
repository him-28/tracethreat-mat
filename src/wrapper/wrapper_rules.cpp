
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
			
				//shared ptr handler code.
				boost::shared_ptr<typename Compiler::compiler_wrapper> compiler_ptr;
				compiler_w_ptr.push_back(compiler_ptr);

				//:compiler_ptr = compiler;
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
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_create(
            typename Compiler::compiler_wrapper * compiler)
    {
        logger->write_info("--Wrapper rules load, compiler create...");
				int errors;
				typename Compiler::compiler_wrapper * cw  = compiler;
        if(yr_compiler_create(&cw) != ERROR_SUCCESS) {
            return false;
        }
				this->compiler = cw;

        return true;
    }

    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_push_fn(
            typename Compiler::compiler_wrapper * compiler,
            char const *file_name_rules)
    {
				int errors = 0;
				//const char * rule_file_conv = (const char*)rule_file;
				FILE * rule_files = fopen("/home/chatsiri/Dropbox/reversing_engineer/malwarecookbook/3/3/clamav.yara", "r");
        if(rule_files != NULL) {
					typename Compiler::compiler_wrapper * cw = compiler;
					errors = yr_compiler_push_file_name(this->compiler, file_name_rules);
					
					if(errors == 0){
						return true;
					}
        }
			return false;
    }


    template class wrapper_rule_compiles<YARA_wrapper>;

}
