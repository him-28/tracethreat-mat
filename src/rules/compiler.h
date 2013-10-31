#ifndef _COMPILER_H
#define _COMPILER_H

#include <stdio.h>

#include "yara.h"


#define yr_compiler_set_error_extra_info(compiler, info) \
    strncpy( \
        compiler->last_error_extra_info, \
        info, \
        sizeof(compiler->last_error_extra_info)); \
    compiler->last_error_extra_info[ \
        sizeof(compiler->last_error_extra_info) - 1] = 0;


int _yr_compiler_push_file(
    YARA_COMPILER* compiler,
    FILE* fh);


FILE* _yr_compiler_pop_file(
    YARA_COMPILER* compiler);

#endif
