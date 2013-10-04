#ifndef UTIL__FILE_CALCULATE_HPP
#define UTIL__FILE_CALCULATE_HPP

/*                       Titles                                          Authors                        Date
 * - Calculate file types send to Oclbuffer                              R.Chatsiri
 */
#define MAX_FILE_INCLUDED 1024


#include "dirent.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include <list>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "gtest/gtest.h"

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
			unsigned int  file_size();
			unsigned int  get_count_file();
			const char ** get_file_cal();
			long  int  ** get_size_cal();
			~file_calculate();
private:
			std::list<std::string> files;
			char * file_path;
			DIR * dir;
			struct dirent *ent;
			// file get size
			FILE * p_file;
			const char * file_name;	
			const char * file_cal[MAX_FILE_INCLUDED];
		  long int   * size_cal[MAX_FILE_INCLUDED];
			unsigned int count_file;
};

}

#endif /* UTIL_FILE_CALCULATE_HPP */
