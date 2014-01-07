#ifndef WRAPPER__WRAPPER_RULES_HPP
#define WRAPPER__WRAPPER_RULES_HPP
/*                       Titles                                          Authors                        Date
 * Add rule files to hnmav prog                                          R.Chatsiri
 *
 */

//extern "C" {
#include "rules/yara.h"
//}


#include "utils/logger/clutil_logger.hpp"
#include "utils/file_handler.hpp"
#include "boost/shared_ptr.hpp"

namespace wrapper
{
    namespace h_util = hnmav_util;
		namespace utility = util;
    /*   Wrapper class call rules for file signature */

    typedef struct _EXTERNAL {
        char type;
        char  *name;
        union {
            char *string;
            int integer;
            int boolean;
        };
        struct _EXTERNAL *next;

    } EXTERNAL;


    struct YARA_wrapper {
        typedef struct _YR_COMPILER  compiler_wrapper;
        typedef struct _YR_RULES  rules_wrapper;
        typedef struct _EXTERNAL   enternal_wrapper;
    };

    template<typename YARA_wrapper>
    class iwrapper
    {
						virtual void set_compiler(typename YARA_wrapper::compiler_wrapper * compiler) = 0;
            virtual typename YARA_wrapper::compiler_wrapper * get_compiler()const = 0;
    };

    template<typename Compiler = struct YARA_wrapper>
    class wrapper_rule_compiles : public iwrapper<Compiler>
    {
        public:
            wrapper_rule_compiles();
            bool compile_rule(char **filerule);
            typename Compiler::compiler_wrapper * get_compiler()const{
								return  this->compiler;
            };
            typename Compiler::rules_wrapper * get_rules() {
                return this->rules;
            };
            bool wrapper_yr_rules_load(const char *filename, typename Compiler::rules_wrapper * rules);

						bool wrapper_yr_compiler_create(typename Compiler::compiler_wrapper * compiler);

						bool wrapper_yr_compiler_push_fn(typename Compiler::compiler_wrapper * compiler,
								char const * file_name_rules);
						
						bool wrapper_yr_compiler_add_file(typename Compiler::compiler_wrapper * compiler,
								char const * rule_file);
							
						/*			
						bool wrapper_yr_compiler_get_rules(typename Compiler::compiler_wrapper * compiler, 
								typename Compiler::rules_wrapper * rules);
						*/

						void set_compiler(typename Compiler::compiler_wrapper * compiler){ 
										this->compiler = compiler;
						}
			
            EXTERNAL *load_rule()const;

        private:
            typename Compiler::compiler_wrapper *compiler;
            typename Compiler::rules_wrapper *rules;
            typename Compiler::enternal_wrapper *external;

            FILE *rule_file;
            char const *argv;
            int pid;
            int errors;
            int result;

						//load file
						utility::ifile<utility::common_filetype> * file_inf;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

}


#endif
