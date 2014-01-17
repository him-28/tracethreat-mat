#ifndef  UTIL__FILE_HANDLER_HPP
#define  UTIL__FILE_HANDLER_HPP

#include <iostream>
#include "filetypes/binary.hpp" 

namespace util{


struct common_filetype{
	typedef FILE file_ptr;
	typedef const char  const_char;
	typedef struct binary_type binary; 
};

template<typename StructFileType, typename PointerType>
struct common_stream_filetype{
		typedef StructFileType s_filetype; 
		typedef streampos s_position;
		typedef PointerType file_ptr;
};

template<typename FileType = struct common_filetype>
class ifile{
public:
	virtual bool file_read() = 0;
  virtual bool set_filepath(char const * file_path) = 0;
	virtual typename FileType::file_ptr * get_file() const = 0;
}; 

template<typename FileType = struct common_filetype>
class file_handler : public ifile<FileType>
{

public:

	file_handler(){ };
  bool file_read();
  bool set_filepath(char const * file_path);
	typename FileType::file_ptr * get_file() const;
	bool get_fdetail_create();	

private:
	typename FileType::filename * p_file;
	const char * file_path;
};

// CPP supported stream file.
template<typename FileType = struct common_stream_filetype>
class file_stream_handler : public ifile<FileType>
{

public:

	file_stream_handler(){ };
  bool file_read();
  bool set_filepath(char const * file_path);
	typename FileType::file_ptr * get_file() const;
	typename FileType::s_position & get_begin() const;
	typename FileType::s_position & get_end() const;	
private:
	typename FileType::s_filetype sf;
	const char * file_path;
	std::ifstream file_read;
};



}


#endif /* UTIL__FILE_HANDLER_HPP */
