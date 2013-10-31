
#ifndef _HASH_H
#define _HASH_H

#include "yara.h"

int yr_hash_table_create(
    int size,
    HASH_TABLE** table);


void yr_hash_table_destroy(
    HASH_TABLE* table);


void* yr_hash_table_lookup(
    HASH_TABLE* table,
    const char* key,
    const char* namespace);


int yr_hash_table_add(
    HASH_TABLE* table,
    const char* key,
    const char* namespace,
    void* value);

#endif
