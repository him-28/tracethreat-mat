#ifndef WRAPPER__WRAPPER_RULES_HPP
#define WRAPPER__WRAPPER_RULES_HPP
/*                       Titles                                          Authors                        Date
 * Add rule files to hnmav prog                                          R.Chatsiri
 *
 */

extern "C"{
//	#include "rules/rules.c"
	#include "rules/yara.h"
}

namespace wrapper{

/*   Wrapper class call rules for file signature */
//struct yc_wrapper_rules;

struct YARA_wrapper{
 	typedef struct YARA_COMPILER  compiler_wrapper;
	typedef struct _YR_RULES  rules_wrapper;
};

typedef struct _EXTERNAL
{
  char type;
  char*  name;
  union {
    char* string;
    int integer;
    int boolean;
  };
  struct _EXTERNAL* next;

} EXTERNAL;

template<typename YARA_wrapper>
class iwrapper
{
public:
	virtual bool compile_rule(char ** filename) = 0;
	virtual typename YARA_wrapper::compiler_wrapper & get_compiler() = 0;
};

template<typename Compiler = struct YARA_wrapper>
class wrapper_rule_compiles : public iwrapper<Compiler>
{
public:
	wrapper_rule_compiles(){ };
	bool compile_rule(char ** filerule);
 	typename Compiler::compiler_wrapper & get_compiler(){ };
	typename Compiler::rules_wrapper * get_rules(){ return rules; };
	bool wrapper_yr_rules_load(const char * filename, typename Compiler::rules_wrapper * rules);	
	EXTERNAL * load_rule()const;
  
private:
  YARA_COMPILER* compiler;
  YR_RULES* rules;
  FILE* rule_file;
  EXTERNAL* external;
	char const * argv;
  int pid;
  int errors;
  int result;

};

}


#endif 
