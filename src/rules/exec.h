
#ifndef _EXEC_H
#define _EXEC_H

#include "yara.h"


#define UNDEFINED           0xFABADAFABADALL
#define IS_UNDEFINED(x)     ((x) == UNDEFINED)

#define HALT        255

#define AND         4
#define OR          5
#define XOR         6
#define NOT         7
#define LT          8
#define GT          9
#define LE          10
#define GE          11
#define EQ          12
#define NEQ         13
#define ADD         14
#define SUB         15
#define MUL         16
#define DIV         17
#define MOD         18
#define NEG         19
#define SHL         20
#define SHR         21
#define PUSH        22
#define POP         23
#define RULE_PUSH   24
#define RULE_POP    25
#define SCOUNT      26
#define SFOUND      27
#define SFOUND_AT   28
#define SFOUND_IN   29
#define SOFFSET     30
#define OF          31
#define EXT_BOOL    32
#define EXT_INT     33
#define EXT_STR     34
#define INCR_M      35
#define CLEAR_M     36
#define ADD_M       37
#define POP_M       38
#define PUSH_M      39
#define SWAPUNDEF   40
#define JNUNDEF     41
#define JLE         42
#define SIZE        43
#define ENTRYPOINT  44
#define INT8        45
#define INT16       46
#define INT32       47
#define UINT8       48
#define UINT16      49
#define UINT32      50
#define CONTAINS    51
#define MATCHES     52


typedef struct _EVALUATION_CONTEXT
{
  uint64_t  file_size;
  uint64_t  entry_point;

  MEMORY_BLOCK*   mem_block;

} EVALUATION_CONTEXT;


int yr_execute_code(
    YARA_RULES* rules,
    EVALUATION_CONTEXT* context);

#endif

