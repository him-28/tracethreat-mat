#ifndef SCAN_FILE_POLICY__HPP
#define SCAN_FILE_POLICY__HPP

template<typename MAPPED_FILE>
struct file_scan_result{
		const char * file_name;
		size_t       file_size;
		MAPPED_FILE  file_detail;

};

template<typename MAPPED_FILE>
class file_policy{
	public:
virtual	std::vector<struct file_scan_result<MAPPED_FILE> > 
	scan_file_engine(std::vector<MAPPED_FILE*> mapped_file_pe) = 0;
};

template<typename MAPPED_FILE>
class file_controller_policy
{

public:

private:
	file_policy<MAPPED_FILE> * f_policy;

};

#endif
