#ifndef FILE_OFFSET_HANDLER__HPP
#define FILE_OFFSET_HANDLER__HPP
/*                       Titles                                          Authors                        Date
 *- File_offset_handler gets offset from file mapped.                    R.Chatsiri                     05/01/2014
 */

namespace util
{

    template<typename FileType = struct common_filetype>
    class pack_file_offset
    {
        public:
						std::vector<int> 

        private:
            std::vector<std::string> offset;
						std::string  file_name;
    };

    template<typename FileType = struct common_filetype>
    class file_offset_handler : public file_handler<FileType>
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
