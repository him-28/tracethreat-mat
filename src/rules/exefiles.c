
#include <limits.h>

#ifdef WIN32
#include <windows.h>
#else
#include "pe.h"
#endif

#include "elf.h"
#include "exec.h"


#ifndef NULL
#define NULL 0
#endif

#ifndef MIN
#define MIN(x,y) ((x < y)?(x):(y))
#endif


PIMAGE_NT_HEADERS yr_get_pe_header(
    uint8_t* buffer,
    size_t buffer_length)
{
  PIMAGE_DOS_HEADER mz_header;
  PIMAGE_NT_HEADERS pe_header;

  size_t headers_size = 0;

  if (buffer_length < sizeof(IMAGE_DOS_HEADER))
    return NULL;

  mz_header = (PIMAGE_DOS_HEADER) buffer;

  if (mz_header->e_magic != IMAGE_DOS_SIGNATURE)
    return NULL;

  if (mz_header->e_lfanew < 0)
    return NULL;

  headers_size = mz_header->e_lfanew + \
                 sizeof(pe_header->Signature) + \
                 sizeof(IMAGE_FILE_HEADER);

  if (buffer_length < headers_size)
    return NULL;

  pe_header = (PIMAGE_NT_HEADERS) (buffer + mz_header->e_lfanew);

  headers_size += pe_header->FileHeader.SizeOfOptionalHeader;

  if (pe_header->Signature == IMAGE_NT_SIGNATURE &&
      pe_header->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 &&
      buffer_length > headers_size)
  {
    return pe_header;
  }
  else
  {
    return NULL;
  }
}


uint64_t yr_pe_rva_to_offset(
    PIMAGE_NT_HEADERS pe_header,
    uint64_t rva,
    size_t buffer_length)
{
  int i = 0;
  PIMAGE_SECTION_HEADER section;

  section = IMAGE_FIRST_SECTION(pe_header);

  while(i < MIN(pe_header->FileHeader.NumberOfSections, 60))
  {
    if ((uint8_t*) section - \
        (uint8_t*) pe_header + sizeof(IMAGE_SECTION_HEADER) < buffer_length)
    {
      if (rva >= section->VirtualAddress &&
          rva <  section->VirtualAddress + section->SizeOfRawData)
      {
        return section->PointerToRawData + (rva - section->VirtualAddress);
      }

      section++;
      i++;
    }
    else
    {
      break;
    }
  }

  return 0;
}


int yr_get_elf_type(
    uint8_t* buffer,
    size_t buffer_length)
{
  Elf32_Ehdr* elf_header;

  if (buffer_length < sizeof(Elf32_Ehdr))
  return 0;

  elf_header = (Elf32_Ehdr*) buffer;

  if (elf_header->e_ident[0] == ELFMAG0 &&
      elf_header->e_ident[1] == ELFMAG1 &&
      elf_header->e_ident[2] == ELFMAG2 &&
      elf_header->e_ident[3] == ELFMAG3)
  {
    return elf_header->e_ident[4];
  }
  else
  {
    return 0;
  }
}


uint64_t yr_elf_rva_to_offset_32(
    Elf32_Ehdr* elf_header,
    uint64_t rva,
    size_t buffer_length)
{
  int i;
  Elf32_Shdr* section;

  if (elf_header->e_shoff == 0 || elf_header->e_shnum == 0)
    return 0;

  // check to prevent integer wraps
  if(ULONG_MAX - elf_header->e_shoff < sizeof(Elf32_Shdr) * elf_header->e_shnum)
    return 0;

  if (elf_header->e_shoff + \
      sizeof(Elf32_Shdr) * elf_header->e_shnum > buffer_length)
    return 0;

  section = (Elf32_Shdr*) ((unsigned char*) elf_header + elf_header->e_shoff);

  for (i = 0; i < elf_header->e_shnum; i++)
  {
    if (section->sh_type != SHT_NULL &&
        section->sh_type != SHT_NOBITS &&
        rva >= section->sh_addr &&
        rva <  section->sh_addr + section->sh_size)
    {
      // prevent integer wrapping with the return value
      if (ULONG_MAX - section->sh_offset < (rva - section->sh_addr))
        return 0;
      else
        return section->sh_offset + (rva - section->sh_addr);
    }

    section++;
  }

  return 0;

}


uint64_t yr_elf_rva_to_offset_64(
    Elf64_Ehdr* elf_header,
    uint64_t rva,
    size_t buffer_length)
{
  int i;
  Elf64_Shdr* section;

  if (elf_header->e_shoff == 0 || elf_header->e_shnum == 0)
    return 0;

  if (elf_header->e_shoff + sizeof(Elf64_Shdr) * elf_header->e_shnum > \
      buffer_length)
    return 0;

  section = (Elf64_Shdr*) ((uint8_t*) elf_header + elf_header->e_shoff);

  for (i = 0; i < elf_header->e_shnum; i++)
  {
    if (section->sh_type != SHT_NULL &&
        section->sh_type != SHT_NOBITS &&
        rva >= section->sh_addr &&
        rva <  section->sh_addr + section->sh_size)
    {
      return section->sh_offset + (rva - section->sh_addr);
    }

    section++;
  }

  return 0;
}


uint64_t yr_get_entry_point_offset(
    uint8_t* buffer,
    size_t buffer_length)
{
  PIMAGE_NT_HEADERS pe_header;
  Elf32_Ehdr* elf_header32;
  Elf64_Ehdr* elf_header64;

  pe_header = yr_get_pe_header(buffer, buffer_length);

  if (pe_header != NULL)
  {
    return yr_pe_rva_to_offset(
        pe_header,
        pe_header->OptionalHeader.AddressOfEntryPoint,
        buffer_length - ((uint8_t*) pe_header - buffer));
  }

  switch(yr_get_elf_type(buffer, buffer_length))
  {
    case ELFCLASS32:
      elf_header32 = (Elf32_Ehdr*) buffer;
      return yr_elf_rva_to_offset_32(
          elf_header32,
          elf_header32->e_entry,
          buffer_length);

    case ELFCLASS64:
      elf_header64 = (Elf64_Ehdr*) buffer;
      return yr_elf_rva_to_offset_64(
          elf_header64,
          elf_header64->e_entry,
          buffer_length);
  }

  return UNDEFINED;
}


uint64_t yr_get_entry_point_address(
    uint8_t* buffer,
    size_t buffer_length,
    size_t base_address)
{
  PIMAGE_NT_HEADERS pe_header;

  Elf32_Ehdr* elf_header32;
  Elf64_Ehdr* elf_header64;

  pe_header = yr_get_pe_header(buffer, buffer_length);

  // If file is PE but not a DLL.

  if (pe_header != NULL &&
      !(pe_header->FileHeader.Characteristics & IMAGE_FILE_DLL))
    return base_address + pe_header->OptionalHeader.AddressOfEntryPoint;

  // If file is executable ELF, not shared library.

  switch(yr_get_elf_type(buffer, buffer_length))
  {
    case ELFCLASS32:
      elf_header32 = (Elf32_Ehdr*) buffer;

      if (elf_header32->e_type == ET_EXEC)
        return elf_header32->e_entry;

      break;

    case ELFCLASS64:
      elf_header64 = (Elf64_Ehdr*) buffer;

      if (elf_header64->e_type == ET_EXEC)
        return elf_header64->e_entry;

      break;
  }

  return UNDEFINED;
}


int yr_file_is_pe(
    uint8_t* buffer,
    size_t buffer_length)
{
  return (yr_get_pe_header(buffer, buffer_length) != NULL);
}


int yr_file_is_elf(
    uint8_t* buffer,
    size_t buffer_length)
{
  int type = yr_get_elf_type(buffer, buffer_length);

  return (type == ELFCLASS32 || type == ELFCLASS64);
}



