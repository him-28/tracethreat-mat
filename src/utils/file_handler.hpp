#ifndef  UTIL__FILE_HANDLER_HPP
#define  UTIL__FILE_HANDLER_HPP

#include <iostream>
#include "filetypes/binary.hpp" 

namespace util{


struct common_filetype{
	typedef FILE filename;
	typedef const char  const_char;
	typedef struct binary_type binary; 
};

template<typename FileType = struct common_filetype>
class ifile{
public:
	virtual bool file_read() = 0;
  virtual bool set_filepath(char const * file_path) = 0;
	virtual typename FileType::filename * get_file() const = 0;
}; 

template<typename FileType = struct common_filetype>
class file_handler : public ifile<FileType>
{

public:

	file_handler(){ };
  bool file_read();
  bool set_filepath(char const * file_path);
	typename FileType::filename * get_file() const;
	bool get_fdetail_create();	
private:
	typename FileType::filename * p_file;
	const char * file_path;
};

}


#endif /* UTIL__FILE_HANDLER_HPP */
