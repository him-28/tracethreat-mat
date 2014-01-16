#ifndef FILE_OFFSET_HANDLER__HPP
#define FILE_OFFSET_HANDLER__HPP
/*                       Titles                                          Authors                        Date
 *- File_offset_handler gets offset from file mapped.                    R.Chatsiri                     05/01/2014
 */

#define FILE_DESCRIPTOR  int


namespace util
{
    struct MAPPED_FILE_PE {
        FILE_DESCRIPTOR   file;
        size_t            size;
        uint8_t 					*data;
    };

    struct MAMORY_BLOCK_PE {
        unsigned char *data;
        size_t				 	size;
        size_t					base;
    };

    template<typename  MAPPED_FILE = struct MAPPED_FILE_PE,
             typename MEMORY_BLOCK = struct MEMORY_BLOCK_PE>
    class file_detail
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

    template<typename FileType = struct common_filetype>
    class file_offset_handler : public file_handler<FileType>, pack_file_offset<FileType>
    {
        public:
            /**
            * @brief File pointer points file name in order to scanning.
            *
            * @param p_file pointer from file_handler(base class)
            *
            * @return True, if able get all offset.
            */
            bool set_file_offset(FileType::filename *p_file);
            /**
            * @brief Seek file from file-mapped.
            *
            * @return File packs offset from seek file.
            */
            std::vector<pack_file_offset>  offset_perfile()const;


        private:

            ifile<common_filetype> *ifh;

    };



}



#endif
