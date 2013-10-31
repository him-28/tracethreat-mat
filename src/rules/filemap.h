
#ifdef WIN32
#include <windows.h>
#define FILE_DESCRIPTOR         HANDLE
#else
#define FILE_DESCRIPTOR         int
#endif

#include <stdlib.h>
#include <stdint.h>


typedef struct _MAPPED_FILE
{
  FILE_DESCRIPTOR     file;
  size_t              size;
  uint8_t*            data;
  #ifdef WIN32
  HANDLE              mapping;
  #endif

} MAPPED_FILE;


int yr_filemap_map(
    const char* file_path,
    MAPPED_FILE* pmapped_file);

void yr_filemap_unmap(
    MAPPED_FILE* pmapped_file);
