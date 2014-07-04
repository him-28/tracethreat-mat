#ifndef  UTIL__FILE_HANDLER_HPP
#define  UTIL__FILE_HANDLER_HPP

#include <iostream>
#include <fstream>

#include "filetypes/binary.hpp"
#include "filetypes/pe.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdexcept>
#include <stdio.h>

#include "utils/logger/clutil_logger.hpp"

namespace utils
{

    namespace h_util = hnmav_util;

    struct common_filetype {
        typedef FILE file_ptr;
        typedef int  file_open_ptr;
        typedef const char  const_char;
        typedef struct binary_type binary;
    };

    struct common_openfile_type {
        typedef int    file_ptr;
        typedef const char  const_char;
        typedef struct binary_type binary;
    };


    template<typename StructFileType, typename PointerType>
    struct common_stream_filetype {
        typedef StructFileType s_filetype;
        typedef std::streampos s_position;
        typedef PointerType file_ptr;
    };

    template<typename FileType = struct common_filetype>
    class ifile
    {
        public:
            virtual bool file_read() = 0;
            virtual bool set_filepath(char const *file_path) = 0;
            virtual typename FileType::file_ptr *get_file() const = 0;
    };

    template<typename FileType = struct common_filetype>
    class file_handler : public ifile<FileType>
    {

        public:

            file_handler() { };
            virtual bool file_read();
            virtual bool set_filepath(char const *file_path);
            virtual typename FileType::file_ptr *get_file() const;

            bool file_read_mapped();
            bool get_fdetail_create();
            struct stat *get_file_status();
            typename FileType::file_open_ptr& get_popen_file();
            bool close_file();

            /**
            * @brief Insert directory path for get all path of file.
            *
            * @param dir_path  Directory path.
            *
            * @return True if get all path in directory success.
            */
            bool file_full_path(const char *dir_path);


						std::vector<std::string> get_full_path();
        private:
            typename FileType::file_ptr *p_file;
            typename FileType::file_open_ptr p_open_file;
            struct stat  file_status;
            const char *file_path;
						std::vector<std::string> file_path_vec;

						boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };

    // CPP supported stream file.
    template<typename StructFileType, typename PointerType,
             typename FileType = struct common_stream_filetype<StructFileType,PointerType > >
    class file_stream_handler : public ifile<FileType>
    {

        public:

            file_stream_handler() { };
            bool file_read();
            bool set_filepath(char const *file_path);

            typename FileType::file_ptr *get_file() const {
                return const_cast<typename FileType::file_ptr *>(p2file); // pointer to file description
            }
            typename FileType::s_position& get_begin() const;
            typename FileType::s_position& get_end() const;
        private:
            typename FileType::s_filetype sf;
            typename FileType::file_ptr *p2file;
            const char *file_path;
            std::ifstream file_stream_read;
    };



}


#endif /* UTIL__FILE_HANDLER_HPP */
