#ifndef FILE_PE_HANDLER__HPP
#define FILE_PE_HANDLER__HPP  

/*                       Titles                                          Authors                        Date
 *-
 *
 */
namespace utils
{

template<typename PEFileType = struct pe_filetype>
class file_pe_reader{
public:

	bool set_pe_offset(std::vector<uint8_t> * vec_pe_offset);

private:
	std::vector<boost::shared_ptr<std::vector<uint8_t> > vec_pe_offset_shared_ptr;

};


};

#endif

