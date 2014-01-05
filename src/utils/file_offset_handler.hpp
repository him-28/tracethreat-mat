#ifndef FILE_OFFSET_HANDLER__HPP
#define FILE_OFFSET_HANDLER__HPP

namespace util
{

		template<typename FileType = struct common_filetype>
		class pack_file_offset{
				public:


				private:
					std::vector<std::string> filename;	
		};

    template<typename FileType = struct common_filetype>
    class file_offset_handler : public file_handler<FileType>
    {
        public:
						std::vector<int>  get_file_offset();
						bool set_file_offset(FileType::filename * p_file);
					  std::vector<pack_file_offset>
        private:

            ifile<common_filetype> * ifh;

    };



}



#endif
