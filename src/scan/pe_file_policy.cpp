/* 						Titles														Authors 												Date
 * Pe file scanning detected malware with get offset scans signature.
 *                                               R.Chatsiri                     24/01/2014
 */

#include "scan/pe_file_policy.hpp"

namespace policy
{

    template<typename MAPPED_FILE>
    pe_file_policy<MAPPED_FILE>::pe_file_policy()
    {
        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        logger->write_info_test("Init logger pe_file_policy");

    }

    // scan_file_type member function have arguement supported single parameter.
    // We plant to supported std::vector<MAPPED_FILE> contains multiple mapped files.
    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::
    scan_file_type(MAPPED_FILE *mapped_file_pe)
    {
        //scan single file only. scan_file_type support single file.
        //Member function get_pe_header and
        std::vector<MAPPED_FILE * >  mapped_file_vec;
        mapped_file_vec.push_back(mapped_file_pe);
        //get pe header file.
        //vector send get_pe_header support multiple file.
        std::vector<struct IMAGE_NT_HEADERS *> nt_header = 
					pe_fconl.get_pe_header(&mapped_file_vec);
			
        MAPPED_FILE * map_file =  mapped_file_vec.back();
				struct IMAGE_NT_HEADERS * nth = nt_header.back();

        struct IMAGE_NT_HEADERS_EXT nt_header_ext =
                pe_fconl.retrive_offset(map_file, nth);
        int count_size_buffer = 0;
        bool ret = true;

        /**
        * @brief Convert data from offset to buffer uint64_t type.
        *
        * @param nt_header_ext->data  Pointer data from file buffer.
        * @param nt_header_ext->size  Size of data
        *
        * @return True, if data can insert to vector contains buffer.
        */
        ret = pe_fconl.convert2buffer(&nt_header_ext.data, nt_header_ext.size);

        if(!ret) {
            logger->write_info("pe_file_policy::scan_file_type, pe_fconl.convert2buffer cannot create buffer");
            return false;
        }

        ret  = pe_fconl.scan(pe_fconl.get_file_buffer());

        if(!ret) {
            logger->write_info("pe_file_policy::scan_file_type, scan cannot scan file buffer");
        }
		
        return true;// scan completed
    }

    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::
    load_plugins_type(MAPPED_FILE   *mapped_file_pe)
    {


    }

    template<typename MAPPED_FILE>
    struct MemoryBlockContext *pe_file_policy<MAPPED_FILE>::
    entry_point_offset(struct MEMORY_BLOCK_PE *memblock) {


    }

    template<typename MAPPED_FILE>
    std::vector<MAPPED_FILE *> *pe_file_policy<MAPPED_FILE>::get_mapped_file()
    {
        return &mapped_files_vec;
    }

    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::set_mapped_file(MAPPED_FILE *mapped_file)
    {
        mapped_files_vec.push_back(mapped_file);
    }

    template class pe_file_policy<struct MAPPED_FILE_PE>;

}


