#ifndef FILE_OFFSET_HANDLER__HPP
#define FILE_OFFSET_HANDLER__HPP
/*                       Titles                                          Authors                        Date
 *- File_offset_handler gets offset from file mapped.                    R.Chatsiri                     05/01/2014
 */

//#define FILE_DESCRIPTOR  int

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <list>
#include "sys/mman.h"

#include "filetypes/pe.hpp"
#include "utils/file_handler.hpp"
#include "exception/system_exception.hpp"

//
#include "utils/logger/clutil_logger.hpp"

namespace util
{
    namespace h_util = hnmav_util;

    namespace file_system_excep = hnmav_exception::file_system;

    template<typename  MAPPED_FILE = struct MAPPED_FILE_PE,
             typename MEMORY_BLOCK = struct MEMORY_BLOCK_PE>
    class file_offset_detail
    {
        public:
            std::vector<MAPPED_FILE *>& get_mappedfile_vec() {
                return mapped_file_vec_ptr;
            }

            std::vector<MEMORY_BLOCK *>& get_memory_block_vec() {
                return memory_block_vec_ptr;
            }
        private:
            std::vector<MAPPED_FILE *> mapped_file_vec_ptr;
            std::vector<MEMORY_BLOCK *> memory_block_vec_ptr;
    };

    template<typename FileType = struct common_filetype>
    class pack_file_offset
    {
        public:
            std::vector<uint8_t> get_offset_pack() const;

        private:
            std::vector<uint8_t> offset;
            std::string  file_name;
    };

    template<typename FileType = struct common_filetype, typename MAPPED_FILE = struct MAPPED_FILE_PE>
    class file_offset_handler : public file_handler<FileType>, public pack_file_offset<FileType>
    {
        public:

            file_offset_handler();
            /**
            * @brief File pointer points file name in order to scanning.
            *
            * @param p_file pointer from file_handler(base class)
            *
            * @return True, if able get all offset.
            */
            bool set_file_offset(typename FileType::file_ptr *p_file);
            /**
            * @brief Seek file from file-mapped.
            *
            * @return File packs offset from seek file.
            */
            std::vector<pack_file_offset<FileType> >  offset_perfile()const;

            /**
            * @brief
            *
            * @param file_name input file name or directory file.
            * @param mapped_vec structure MAPPED_FILE type container.
            *
            * @return True, success init file mapped.
            */
            bool mapped_file(std::list<std::string> file_name,
                    std::vector<MAPPED_FILE *> mapped_vec,
                    file_offset_handler<FileType, MAPPED_FILE>& file_offset_object);

            bool unmapped_file(std::vector<MAPPED_FILE * > mapped_vec);
        private:

            ifile<common_filetype> *ifh;
			      std::vector<boost::shared_ptr<std::vector<MAPPED_FILE *> > >  mapped_file_vec_shared;
            std::vector<MAPPED_FILE *>   *mapped_vec_;
            MAPPED_FILE *mapped_file_ptr;
            //logger
            boost::shared_ptr<h_util::clutil_logging<std::string, int> > *logger_ptr;
            h_util::clutil_logging<std::string, int>    *logger;

    };



}



#endif
