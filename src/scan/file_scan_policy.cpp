
#include "scan/file_scan_policy.hpp"
//#include "filetypes/pe_file_controller.hpp"

namespace policy
{

    template<typename MAPPED_FILE>
    struct file_scan_result;

    template<typename MAPPED_FILE>
    pe_file_policy<MAPPED_FILE>::pe_file_policy()
    {
			 
        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        logger->write_info_test("Init logger pe_file_policy");

    }

		template<typename MAPPED_FILE>
		pe_file_policy<MAPPED_FILE>::~pe_file_policy(){ }

    // scan_file_type member function have arguement supported single parameter.
    // We plant to supported std::vector<MAPPED_FILE> contains multiple mapped files.
    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::
    scan_file_type(MAPPED_FILE *mapped_file_pe)
    {
	
				logger->write_info("Start pe_file_policy<MAPPED_FILE>::scan_file_type");
		
        //scan single file only. scan_file_type support single file.
        //Member function get_pe_header and
        std::vector<MAPPED_FILE * >  mapped_file_vec;
        mapped_file_vec.push_back(mapped_file_pe);
        //get pe header file.
        //vector send get_pe_header support multiple file.
        std::vector<struct IMAGE_NT_HEADERS *> nt_header =
                pe_fconl.get_pe_header(&mapped_file_vec);

				logger->write_info_test("pe_file_policy<MAPPED_FILE>::scan_file_type, return nt_header size ",
						boost::lexical_cast<std::string>(nt_header.size()));

        MAPPED_FILE *mapped_file =  mapped_file_vec.back();
			  if(mapped_file == NULL)
						logger->write_info_test("pe_file_policy<MAPPED_FILE>::scan_file_type, mapped_file is zero");
			
        struct IMAGE_NT_HEADERS *nth = nt_header.back();
				if(nth == NULL)
						logger->write_info_test("pe_file_policy<MAPPED_FILE>::scan_file_type, Header is zero");

				//logger->write_info("pe_file_policy<MAPPED_FILE>::scan_file_type, mapped_file data", 
						boost::lexical_cast<std::string>(mapped_file->data);

        struct IMAGE_NT_HEADERS_EXT nt_header_ext =
                pe_fconl.retrive_offset(mapped_file, nth);
        int count_size_buffer = 0;
        bool ret = true;

        /**
        * @brief Convert data from offset to buffer uint64_t type.
        *
        * @param nt_header_ext.data  Pointer data from file buffer.
        * @param nt_header_ext.size  Size of data
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

    template<typename MAPPED_FILE>
    struct file_scan_result<MAPPED_FILE>& pe_file_policy<MAPPED_FILE>::get_result()const {

    }

    

    //---------------------------- File Policy  ------------------------------//
    
    template<typename MAPPED_FILE>
    file_scan_policy<MAPPED_FILE>::file_scan_policy()
    {
    //logger
    logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
    logger = logger_ptr->get();
    logger->write_info("Init logger file_scan_policy");
    }
    

    template<typename MAPPED_FILE>
    std::vector<struct file_scan_result<MAPPED_FILE>* >& file_scan_policy<MAPPED_FILE>::
    scan_file_engine(file_scan_policy<MAPPED_FILE> *fcol_policy)
    {
				//logger->write_info("In file_scan_policy<MAPPED_FILE>::scan_file_engine");		  	
        std::vector<MAPPED_FILE *> *mapped_file_vec =  fcol_policy->get_mapped_file();
        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_file;
        uint8_t result_file_count = 0;
				f_col_policy = fcol_policy;
				f_col_policy->get_result();
			
        for(iter_mapped_file  = mapped_file_vec->begin();
                iter_mapped_file != mapped_file_vec->end();
                ++iter_mapped_file) {
            MAPPED_FILE *mapp_file  =  *iter_mapped_file;

            if(f_col_policy->scan_file_type(mapp_file)) {
                logger->write_info("Scan found infected file ",
                        boost::lexical_cast<std::string>(mapp_file->file_name));

                //create struct file scan result contain on vector.
                fs_result = new file_scan_result<MAPPED_FILE>();
                file_scan_result_vec.push_back(fs_result);
                file_scan_result_vec[result_file_count]->file_name = mapp_file->file_name;

                //count result size of vector
                result_file_count++;
            }

            // TO-DO
            //f_col_policy->load_plugins_type(mapp_file, pl_result);
        }
			
    }
		
		template class pe_file_policy<struct MAPPED_FILE_PE>;

    template class file_scan_policy<struct MAPPED_FILE_PE>;

}

