#ifndef UTIL__FILE_CALCULATE_HPP
#define UTIL__FILE_CALCULATE_HPP

/*                       Titles                                          Authors                        Date
 * - Calculate file types send to Oclbuffer                              R.Chatsiri
 */
//#include <cstdint>
//#include "stdint.h"
#include "dirent.h"
//#include "stddef.h"
#include <list>

#include <iostream>

namespace util
{

struct Extension
{
	unsigned int  elf_t;
	unsigned int  exe_t;
};

template<typename Extension>
class file_calculate 
{
public:
			bool set_filepath(char * file_path);
			bool get_file();
			bool processes();
			unsigned int file_size(const char * filename);
private:
			std::list<Extension> files;
			char * file_path;
			DIR * dir;
			struct dirent *ent;	
};

}

#endif /* UTIL__FILE_CALCULATE_HPP */
