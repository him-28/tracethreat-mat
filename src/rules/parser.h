
#ifndef _PARSER_H
#define _PARSER_H

#include "arena.h"
#include "compiler.h"
#include "lex.h"
#include "sizedstr.h"


int yr_parser_emit(
    yyscan_t yyscanner,
    int8_t instruction,
    int8_t** instruction_address);


int yr_parser_emit_with_arg(
    yyscan_t yyscanner,
    int8_t instruction,
    int64_t argument,
    int8_t** instruction_address);


int yr_parser_emit_with_arg_reloc(
    yyscan_t yyscanner,
    int8_t instruction,
    int64_t argument,
    int8_t** instruction_address);


STRING* yr_parser_lookup_string(
  yyscan_t yyscanner,
  const char* identifier);


EXTERNAL_VARIABLE* yr_parser_lookup_external_variable(
  yyscan_t yyscanner,
  const char* identifier);


void yr_parser_push_string_pointers(
    yyscan_t yyscanner,
    const char* identifier);


int yr_parser_reduce_rule_declaration(
    yyscan_t yyscanner,
    int flags,
    const char* identifier,
    char* tags,
    STRING* strings,
    META* metas);


STRING* yr_parser_reduce_string_declaration(
    yyscan_t yyscanner,
    int flags,
    const char* identifier,
    SIZED_STRING* str);


META* yr_parser_reduce_meta_declaration(
    yyscan_t yyscanner,
    int32_t type,
    const char* identifier,
    const char* string,
    int32_t integer);


int yr_parser_reduce_string_identifier(
    yyscan_t yyscanner,
    const char* identifier,
    int8_t instruction);


int yr_parser_reduce_external(
    yyscan_t yyscanner,
    const char* identifier,
    int8_t intruction);

int yr_parser_lookup_loop_variable(
    yyscan_t yyscanner,
    const char* identifier);

#endif
