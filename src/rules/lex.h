
#include "compiler.h"

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#define YY_EXTRA_TYPE YARA_COMPILER*
#define YY_USE_CONST

void yyerror(
		yyscan_t yyscanner,
		const char *error_message);

YY_EXTRA_TYPE yyget_extra(
		yyscan_t yyscanner);

int yr_lex_parse_rules_string(
		const char* rules_string,
		YARA_COMPILER* compiler);

int yr_lex_parse_rules_file(
		FILE* rules_file,
		YARA_COMPILER* compiler);

