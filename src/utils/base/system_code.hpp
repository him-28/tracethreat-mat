#ifndef UTIL_SYSTEM_CODE_HPP
#define UTIL_SYSTEM_CODE_HPP

// File scanning 
#define FILE_FOUND 								0   /* Scan infected file found */
#define FILE_NOT_FOUND 						1   /* Scan infected file not found */

namespace util{
	typedef	enum SCAN_FILE{ FILE_FOUND , FILE_NOT_FOUND } scan_file_code;

}

#endif
