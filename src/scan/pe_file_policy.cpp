/* 						Titles														Authors 												Date
 * Pe file scanning detected malware with get offset scans signature.
 *                                               R.Chatsiri                     24/01/2014
 */

#include "scan/pe_file_policy.hpp"

template<typename Policy,typename MAPPED_FILE>
bool pe_file_policy<Policy, MAPPED_FILE>::
scan_file_type(MAPPED_FILE *mapped_file_pe)
{
    std::vector<MAPPED_FILE * > mapped_file_vec;
    mapped_file_vec.push_back(mapped_file_pe);
    //get pe header file.
    //vector send get_pe_header support multiple file.
    std::vector<struct IMAGE_NT_HEADER *> nt_header = pe_fconl.get_pe_header(mapped_file_vec);
    struct IMAGE_NT_HEADERS_EXT * nt_header_ext = 	
				pe_fconl.retrive_offset(mapped_file_vec.back(), nt_header);
		int count_size_buffer = 0;
		bool ret = true;

		ret = pe_fconl.convert2buffer(nt_header_ext->data, nt_header_ext->size)

		if(!ret){
			logger->write_info("pe_file_policy::scan_file_type, pe_fconl.convert2buffer cannot create buffer");
					return false;
		}

		ret  = pe_fconl.scan(pe_fconl.file_buffer_vec);

		if(!ret){
			logger->write_info("pe_file_policy::scan_file_type, scan cannot scan file buffer");
		}

	return true;// scan completed
}

template<typename Policy,typename MAPPED_FILE>
std::vector<struct file_scan_result<MAPPED_FILE> > pe_file_policy<Policy, MAPPED_FILE>::
load_plugins_type(MAPPED_FILE   *mapped_file_pe)
{


}

template<typename Policy,typename MAPPED_FILE>
struct MemoryBlockContext *pe_file_policy<Policy, MAPPED_FILE>::
entry_point_offset(struct MEMORY_BLOCK_PE *memblock) {


}

