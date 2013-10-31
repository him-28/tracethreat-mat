
#ifndef _EXEFILES_H
#define _EXEFILES_H

int yr_file_is_pe(
		uint8_t* buffer,
		size_t buffer_length);


int yr_file_is_elf(
		uint8_t* buffer,
		size_t buffer_length);


uint64_t yr_get_entry_point_offset(
		uint8_t* buffer,
		size_t buffer_length);


uint64_t yr_get_entry_point_address(
		uint8_t* buffer,
		size_t buffer_length,
		size_t base_address);

#endif

