
#include "scan/file_scan_policy.hpp"
//#include "filetypes/pe_file_controller.hpp"
#include <sstream>

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
    pe_file_policy<MAPPED_FILE>::~pe_file_policy() { }


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

        if(mapped_file == NULL) {
            logger->write_info_test("pe_file_policy<MAPPED_FILE>::scan_file_type, mapped_file is zero");
        }

        struct IMAGE_NT_HEADERS *nth = nt_header.back();

        if(nth == NULL) {
            logger->write_info_test("pe_file_policy<MAPPED_FILE>::scan_file_type, Header is zero");
        }

        logger->write_info("pe_file_policy<MAPPED_FILE>::scan_file_type, mapped_file data",
                boost::lexical_cast<std::string>(mapped_file->data));

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

        logger->write_info_test("pe_file_policy<MAPPED_FILE>::scan_file_type, TEST only");
        std::string value_str("a824bc647c46d04b537db2e3a33711");
        //nt_header_ext.data_offset;
        unsigned char data_offset[] = "a824bc647c46d04b537db2e3a33711";
        //nt_header_ext.size

        //ret = pe_fconl.convert2buffer(nt_header_ext.data_offset, nt_header_ext.size);

        logger->write_info_test("pe_file_policy<MAPPED_FILE>::scan_file_type, Repace with nt_header_ext");
        //test only
        ret = pe_fconl.convert2buffer(data_offset, value_str.size());

        logger->write_info("pe_file_policy<MAPPED_FILE>::scan_file_type, data_offset size",
                boost::lexical_cast<std::string>(nt_header_ext.size));

        /*
        for(size_t count_data = 0; count_data < nt_header_ext.size; count_data++)
        {
        		 				printf("%x",nt_header_ext.data_offset[count_data]);
        }
        printf("\n");
        */
        if(!ret) {
            logger->write_info("pe_file_policy::scan_file_type, \
															  pe_fconl.convert2buffer cannot create buffer");
            return false;
        }

        /**
        * @brief Set kernel file path to ocl_load_system
        *
        * @param this->kernel_file_path  string contains kernel extension .cl
        *
        * @return True, If file contains kernel path of string.
        */
        ret  = pe_fconl.set_opencl_file(*this->kernel_file_path);

        if(!ret) {
            logger->write_info("pe_file_policy::scan_file_type, cannot open kernel file.");
            return false;
        }

        utils::scan_file_code scanf_code = pe_fconl.scan(this->node_symbol_vec,
                this->node_state_vec,
                &pe_fconl.get_file_buffer());

        if(scanf_code == utils::infected_found) {
            logger->write_info("pe_file_policy::scan_file_type, found file infected.");
        }

        logger->write_info_test("Size of node_symbol ",
                boost::lexical_cast<std::string>(this->node_symbol_vec->size()));

        logger->write_info_test("Size of node_state ",
                boost::lexical_cast<std::string>(this->node_state_vec->size()));



        if(!ret) {
            logger->write_info("pe_file_policy::scan_file_type, scan cannot scan file buffer");
            return false;
        }

        return true;// scan completed
    }


    /**
    * @brief scan_file_type supported vector contains multiple files scanning with OCL
    *
    * @param mapped_file_pe_vec False, If cannot scanning completed.
    */
    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::
    scan_file_type(std::vector<MAPPED_FILE *> *mapped_file_pe_vec)
    {
				logger->write_info("pe_file_policy::scan_file_type(), multiple scanning with ocl");

        pe_fconl.set_opencl_file(*this->kernel_file_path);
        //Plan-00003 : Retrun result is array.
        pe_fconl.scan(this->node_symbol_vec,
                this->node_state_vec,
                mapped_file_pe_vec,
                this->kernel_file_path);
        //Plan-00004 : read result and match with internal arena.
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
        return true;
    }

    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::set_mapped_file(std::vector<MAPPED_FILE *> *mapped_file)
    {
        mapped_files_vec.insert(mapped_files_vec.begin(),
                mapped_file->begin(),
                mapped_file->end());
        return true;
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

        //Policy single and mulitple scanning file TBB.
        for(iter_mapped_file  = mapped_file_vec->begin();
                iter_mapped_file != mapped_file_vec->end();
                ++iter_mapped_file) {
            MAPPED_FILE *mapp_file  =  *iter_mapped_file;

            if(f_col_policy->scan_file_type(mapp_file)) {
                //Mapp_file instant mapped_file
                if(mapp_file->file_name == NULL) {
                    logger->write_info("file_scan_policy::scan_file_engine, Scan found infected file ",
                            boost::lexical_cast<std::string>(mapp_file->file_name));

                    //create struct file scan result contain on vector.
                    fs_result = new file_scan_result<MAPPED_FILE>();
                    file_scan_result_vec.push_back(fs_result);
                    file_scan_result_vec[result_file_count]->file_name = mapp_file->file_name;
                }

                logger->write_info("file_scan_policy::scan_file_engine, Scan found, but unname of file");
                //count result size of vector
                result_file_count++;
            }

            // TO-DO
            //f_col_policy->load_plugins_type(mapp_file, pl_result);
        }// End-for loop

    }

    template<typename MAPPED_FILE>
    std::vector<struct file_scan_result<MAPPED_FILE>* >& file_scan_policy<MAPPED_FILE>::
    scan_file_engine(file_scan_policy<MAPPED_FILE> *fcol_policy,
            std::vector<MAPPED_FILE *> *mapped_file_vec)
    {
        logger->write_info("In file_scan_policy<MAPPED_FILE>::scan_file_engine");

        uint8_t result_file_count = 0;
        f_col_policy = fcol_policy;
        f_col_policy->get_result();

        //return scanning completed all files.
        if(f_col_policy->scan_file_type(mapped_file_vec)) {

            logger->write_info("file_scan_policy::scan_file_engine, Scan found, but unname of file");
        }// end if

        // TO-DO
        //f_col_policy->load_plugins_type(mapp_file, pl_result);

    }


    template class pe_file_policy<struct MAPPED_FILE_PE>;

    template class file_scan_policy<struct MAPPED_FILE_PE>;

}

