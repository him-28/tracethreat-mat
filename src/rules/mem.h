
#ifndef _MEM_H
#define _MEM_H

#include <stdio.h>

void yr_heap_alloc();

void yr_heap_free();

void* yr_malloc(
		size_t size);

void* yr_realloc(
		void* ptr,
		size_t size);

void yr_free(
		void *ptr);

char* yr_strdup(
		const char *str);

#endif


