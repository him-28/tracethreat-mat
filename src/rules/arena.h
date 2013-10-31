#ifndef _ARENA_H
#define _ARENA_H

#include <stdint.h>
#include <stddef.h>

#include "yara.h"

int yr_arena_create(
    ARENA** arena);


void yr_arena_destroy(
    ARENA* arena);


void* yr_arena_base_address(
  ARENA* arena);


void* yr_arena_current_address(
  ARENA* arena);


void* yr_arena_next_address(
  ARENA* arena,
  void* address,
  size_t increment);


int yr_arena_coalesce(
    ARENA* arena);


int yr_arena_allocate_memory(
    ARENA* arena,
    int32_t size,
    void** allocated_memory);


int yr_arena_allocate_struct(
    ARENA* arena,
    int32_t size,
    void** allocated_memory,
    ...);


int yr_arena_make_relocatable(
    ARENA* arena,
    void* base,
    ...);


int yr_arena_write_data(
    ARENA* arena,
    void* data,
    int32_t size,
    void** written_data);


int yr_arena_write_string(
    ARENA* arena,
    const char* string,
    char** written_string);


int yr_arena_append(
    ARENA* target_arena,
    ARENA* source_arena);


#endif


