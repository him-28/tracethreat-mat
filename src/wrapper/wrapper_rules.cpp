
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

        boost::shared_ptr<typename Compiler::rules_wrapper> rules_ptr;
        rules_w_ptr.push_back(rules_ptr);
    }

    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_rules_load(
            const char *filename,
            typename Compiler::rules_wrapper *rules)
    {
        int result;
				std::cout<<"Rules addr : " << rules << ", &Rules addr : " << &rules <<std::endl;
        result = yr_rules_load(filename, &rules /* &this->rules*/);
        logger->write_info("--Wrapper rules load, result : ", boost::lexical_cast<std::string>(result));
				std::cout<<"After Rules addr : " << rules<<", &Rules addr : "<< &rules <<std::endl;
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

						std::cout<<" Compiler addr : " << compiler << ", This->compiler : " << this->compiler <<std::endl;

        if(yr_compiler_create(&this->compiler) != ERROR_SUCCESS) {

            return false;
        }
						std::cout<<"After Compiler addr : " << compiler << ", This->compiler : " << this->compiler <<std::endl;

        return true;
    }

    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_push_fn(
            typename Compiler::compiler_wrapper *compiler,
            char const *file_name_rules)
    {
        int errors = 0;

        if(file_name_rules != NULL) {
						std::cout<<" Compiler addr : " << compiler <<std::endl;
            errors = yr_compiler_push_file_name(compiler, file_name_rules);
						std::cout<<" After Compiler addr : " << compiler <<std::endl;

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
						std::cout<<" Compiler addr : " << compiler <<std::endl;

        int error = yr_compiler_add_file(compiler, file_inf->get_file(), NULL);
						std::cout<<" After Compiler addr : " << compiler <<std::endl;

       // std::cout<<" Compiler addr : " << this->compiler <<std::endl;

        if(error) return false;

        return true;
    }

    /*
    template<typename Compiler>
    bool wrapper_rule_compiles<Compiler>::wrapper_yr_compiler_get_rules(
    		typename Compiler::compiler_wrapper * compiler,
    						typename Compiler::rules_wrapper * rules)
    {
    		std::cout<<" Rules addr : " << rules <<", Rules de addr : " << &rules <<std::endl;
    		std::cout<<" Compiler addr : " << this->compiler <<std::endl;

    		if(!yr_compiler_get_rules(this->compiler, &this->rules)) return true;
    		return false;

    }
    */

    template class wrapper_rule_compiles<YARA_wrapper>;

}
