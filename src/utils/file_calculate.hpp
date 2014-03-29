#ifndef UTIL__FILE_CALCULATE_HPP
#define UTIL__FILE_CALCULATE_HPP

/*                       Titles                                          Authors                        Date
 * - Calculate file types send to Oclbuffer                              R.Chatsiri
 */
#define MAX_FILE_INCLUDED 1024
#define MAX_BUFFER_SIZES  524288

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

namespace utils
{


    struct Extension {
        typedef unsigned int  elf_t;
        typedef unsigned int  exe_t;
    };

    struct file_detail {
        unsigned int size_cal;
        const char   *file_cal;
        file_detail *operator=(file_detail *file_detail) {
            if(file_detail == NULL) {
                return (struct file_detail *)
                        malloc(sizeof(struct file_detail) * MAX_FILE_INCLUDED);
            }

            return file_detail;
        }
    };

    template<typename Extension>
    class file_calculate
    {
        public:
            bool set_filepath(const char *file_path);
            bool get_file();
            bool processes();
            unsigned int  file_size();
            unsigned int  get_count_file();
            struct file_detail *get_file_d();
            ~file_calculate();
						std::list<std::string> & get_files2buffer();
        private:
            std::list<std::string> files;
						std::list<std::string> files2buffer;
            const char *file_path;
            DIR *dir;
            struct dirent *ent;
            // test
            struct file_detail *file_d;
            // file get size
            FILE *p_file;
            const char *file_name;
            unsigned int count_file;
            unsigned int size;
						// filter file send to buffer;
						unsigned int buffer_size;
    };

}

#endif /* UTIL_FILE_CALCULATE_HPP */
