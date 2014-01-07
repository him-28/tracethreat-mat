
/*                       Titles                                          Authors                        Date
 * - Wrapper rules.c                                                     R.Chatsiri
 */

#include "wrapper/wrapper_rules.hpp"
#include "boost/lexical_cast.hpp"

namespace wrapper
{
    //	namespace utility = util;

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
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_create(
            typename Compiler::compiler_wrapper *compiler)
    {
        logger->write_info("--Wrapper rules load, compiler create...");
        int errors;

				/*&this->compiler */
        if(yr_compiler_create(&compiler) != ERROR_SUCCESS) {

            return false;
        }
				this->compiler = compiler;

        return true;
    }

    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_push_fn(
            typename Compiler::compiler_wrapper *compiler,
            char const *file_name_rules)
    {
        int errors = 0;

        if(file_name_rules != NULL) {
            errors = yr_compiler_push_file_name(compiler, file_name_rules);
            if(errors == 0) {
                return true;
            }
        }

        return false;
    }

    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_add_file(
            typename Compiler::compiler_wrapper *compiler,
            char const *file_name_rule)
    {

        file_inf = new utility::file_handler<utility::common_filetype>();
        file_inf->set_filepath(file_name_rule);
        file_inf->file_read();

        int error = yr_compiler_add_file(compiler, file_inf->get_file(), NULL);

        if(error) return false;

        return true;
    }

/*    
    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_get_rules(
    		typename Compiler::compiler_wrapper * compiler,
    						typename Compiler::rules_wrapper * rules)
    {

    		if(!yr_compiler_get_rules(compiler, &this->rules)) return true;
    		return false;

    }
    
*/
    template class wrapper_rule_compiles<YARA_wrapper>;

}
