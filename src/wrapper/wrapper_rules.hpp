#ifndef WRAPPER__WRAPPER_RULES_HPP
#define WRAPPER__WRAPPER_RULES_HPP

extern "C"{
	#include "rules/rules.c"
	#include "rules/yara.h"
}

namespace wrapper{

/*   Wrapper class call rules for file signature */
//struct yc_wrapper_rules;

struct YARA_Rules{
 	typedef struct YARA_COMPILER  compiler_wrapper;
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

template<typename YARA_Rules>
class iwrapper
{
public:
	virtual bool compile_rule(char ** filename) = 0;
	virtual YARA_Rules & get_compiler() = 0;
};

template<typename Compiler>
class wrapper_rule_compiles : public iwrapper<Compiler>
{
public:
	wrapper_rule_compiles(){ };
	bool compile_rule(char ** filerule);
 	YARA_Rules & get_compiler(){ };
	bool wrapper_yr_rules_load(const char * filename, YR_RULES ** rules);	
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
